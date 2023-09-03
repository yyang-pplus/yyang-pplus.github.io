---
title: "PDS: Logging Rate Limiter"
date: "2021-03-11"
categories:
  - "cpp"
tags:
  - "algorithm"
  - "c++"
  - "debugging"
  - "engineering"
  - "logging"
  - "practical-debugging-skills"
  - "programming"
  - "software"
  - "system design"
  - "thread"
---


Logging is an important and integral part of software development. However, just like everything else, logging has its own downside as well. One of the typical issues with logging is actually having too many log entries. In today's post, we are going to talk about the common ways to limit logging into a sensible rate.


# Logging in a nutshell

As a developer, if you have been working on a fairly complex software, the chances are, you probably already known what logging is in programming, and how a carefully constructed log can help you to pinpoint an issue much faster, especially in production. In short, **logging** is the process of recording an application's key actions and their context. Later on, the logged information can help the engineers to mentally reconstruct the states of the application at certain points. A Log can be an important source of information, and sometimes the only source, when diagnosing a problem. As it said, the clues most useful in figuring out something that went wrong often lie in the events immediately prior.

There is no doubt that logging is a useful tool, but, as experienced developers already known, too many log entries can do more harms than no logs at all. For one, logs clustered with irrelevant information can be difficult to browse. For another, logging can consume too much resources, such as, CPU, disk I/O or space, network bandwidth, and so forth, in a short period of time, thus affect the overall performance of the system significantly.

Now we know what kind of problem we are facing; let us talk about how we can resolve it.


# Log severity levels

When doing logging for a program, it is more common to use a logging framework, instead of building the logging system all from scratch. Most logging frameworks provide a mechanism to assign a severity level to each log entry and the ability to filter out log entries of certain levels. Thus, setting a **minimum severity level**, or **log level threshold**, is usually the easiest way to reduce the number of items written to the log.


# Log category

Beside log severity levels, some logging frameworks also support classify log entries into different categories. Log category can usually be used to turn off logging for an entire subsystem. If you image log levels as a way to classify log entries horizontally, then log categories can be seen as a way to divide log entries vertically. One common situation with log level is that, when you try to lower the minimum severity level to allow more items to get logged, and, suddenly, the log is flooded with so much irrelevant information that makes it hard to browse. With log category, however, you can just enable logging of the concerned subsystems, thus limiting the amount of information gets recorded into the log. For this reason, log levels is often combined with log categories to make the best use of both.


# Rate limiters

**Limiting log rate** refers to controlling the number of log entries that are generated over a given period of time. It acts more like a passive defensive mechanism, when compares to log levels and log categories. Also, unlike log levels or log categories, it is relatively rare for a logging framework to have build-in support for rate-limiting mechanisms. So we have to build a logging rate limiter ourselves.

## The logging framework

To keep things simple, I will not use any real logging frameworks available in the market, instead, I will use an imaginary logging library called `logpp`, which has the following interfaces:

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/logpp.hpp %}
```

## Token bucket

The **token bucket** algorithm is based on an analogy of a fixed capacity bucket, with tokens being added into the bucket at a constant rate. Once the bucket is full, no more tokens can be added. When a log message is requested to be logged, the bucket is inspected to see if it contains any tokens at that moment. If it does, one token is removed from the bucket, and the log message passes through. Otherwise, the log message is dropped.[<sup>\[1\]</sup>](#references)

The typical implementation of token bucket algorithm takes two parameters:

* **Bucket size** is the maximum number of tokens the bucket can hold.
* **Refill rate** is the rate at which the bucket gets refilled with tokens.

Thus, a logger applies token bucket algorithm can produce messages with an average rate up to the refill rate, and have a burstiness determined by the bucket size.[<sup>\[1\]</sup>](#references)

The token bucket algorithm can be implemented in C++ like this:

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/rate.hpp %}
```

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/token.hpp %}
```

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/token-bucket.hpp %}
```

Note, the `Token` class only has private constructors, thus only its friend `TokenBucketLimiter` can create new tokens.

A logger that uses token bucket algorithm may look like this:

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/token-bucket-logger.hpp %}
```

## Leaky bucket

The **leaky bucket** is an algorithm based on an analogy of a fixed capacity bucket, with holes that leaks water out of the bucket at a constant rate. Log messages are added into the bucket, which just like the way water is poured into a bucket. When the bucket is full, messages will overflow. Meanwhile, the bucket is inspected at a constant rate to see if it contains any log messages at that moment. If it does, one message is leaked from the bucket and passed through.[<sup>\[2\]</sup>](#references)

There are two versions of the leaky bucket algorithm: the leaky bucket as a meter and the leaky bucket as a queue.

### Leaky bucket as a meter

In the leaky bucket as a meter version, the bucket is implemented as a counter or variable to count the number of log entries currently in the bucket:[<sup>\[2\]</sup>](#references)

* The counter is incremented whenever a conforming log request arrives; this process mirrors that of decreasing the token counter when tokens are removed from the token bucket.
* The counter is decremented periodically; this process mirrors that of regularly increasing the token counter as tokens are added to the token bucket at a fixed rate.
* If the counter exceeds a threshold upon being incremented, the log message is dropped; this test mirrors that of checking the token bucket contains enough tokens upon removing tokens.

As you can see, the leaky bucket as a meter is exactly equivalent to a mirror image of the token bucket algorithm, thus, the implementation of the leaky bucket as a meter is omitted in this article.[<sup>\[2\]</sup>](#references)

### Leaky bucket as a queue

The leaky bucket as a queue can be seen as a special case of the version as a meter.[<sup>\[2\]</sup>](#references) In this version, the bucket is implemented as a finite FIFO buffer or queue of log entries to directly control the log rate:[<sup>\[2\]</sup>](#references)

* Each message is entered into the queue as it is requested to be logged, if the queue is not full at that time.
* Messages are then removed from the queue at a fixed rate for onward processing.

The leaky bucket as a queue algorithm takes the following two parameters:

* **Bucket size** is the maximum number of log entries the bucket can hold.
* **Leak rate** is the rate at which log entries get removed from the bucket for onward processing.

Thus, a logger uses leaky bucket as a queue algorithm can produce messages with an average rate up to the leak rate.

The implementation of the leaky bucket as a queue is more elaborate comparing to other rate limiting algorithms, as its implementation usually involves more than one threads. Below is one possible implementation of a logger that applies the leaky bucket as a queue algorithm:

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/leaky-bucket-logger.hpp %}
```

Note that `Start()` needs to be called manually after a `LeakyBucketLogger` object is instantiated to actually launch the worker thread. This is a safe idiom to have `std::thread` as class members, although not really necessary in our case.[<sup>\[3\]</sup>](#references)

## Fix window counter

The fix window counter algorithm works as follows:

* Divide the time into fix-sized windows and each window has a counter to count the number of log entries within the time window.
* Each conforming log request increments the counter by one.
* The counter is set to zero when a new time window starts.
* If the counter exceeds a threshold upon being incremented, the log message is dropped.

I see the fix window counter algorithm as a special case of the leaky bucket as a meter algorithm, where the bucket size equals to the number of log entries get leaked per time window. For this reason, the implementation of the fix window counter is omitted.

## Sliding window log

The sliding window log algorithm involves tracking the time of logging requests with a log of timestamps. When a new request comes in, timestamps those are older than the start of the current time window are removed from the timestamp log. After that, if the log size is less than the given threshold, the new request passes through and its timestamp is added to the log. Otherwise, the request is dropped.[<sup>\[4\]</sup>](#references)

The sliding window log algorithm takes the following two parameters:

* **Window size** is the size of the time window, beyond which timestamps are removed.
* **Threshold** is the maximum number of timestamps per time window.

Thus, a logger uses sliding window log algorithm can produce messages with an average rate up to the threshold per window.

One possible implementation of the sliding window log algorithm is:

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/sliding-log-logger.hpp %}
```

## Sliding window counter

The sliding window counter algorithm is a variant of the fixed window counter algorithm. It can produce a more smooth flow of log messages when comparing to the fixed window counter algorithm by taking the counter of previous window into account. The sliding window counter algorithm works as follows:[<sup>\[5\]</sup>](#references)

* Divide the time into fix-sized windows and each window has a counter to count the number of log entries within the time window.
* Each conforming log request increments the counter by one.
* The counter is set to zero when a new time window starts.
* The number of log entries in the sliding window is calculated using this formula: the count of current fixed window + the count of previous fixed window * overlap percentage of the sliding window and the previous fixed window.
* If the sliding count exceeds a threshold, the log message is dropped.

The sliding window counter algorithm takes the following two parameters:

* **Window size** is the size of the fixed time window and the sliding window.
* **Threshold** is the maximum number of log requests within the sliding window.

Thus, a logger uses sliding window counter algorithm can produce messages with an average rate approximately up to the threshold per window.

A logger that uses sliding window counter algorithm can be implemented like this:

```cpp
{% include src/2021-03-11-pds-logging-rate-limiter/sliding-window-counter-logger.hpp %}
```

## A few more words

In a perfect world, where software is tested completely and is bug-free, protective mechanisms, like logging rate limiters, should have no use. However, our reality is far from ideal, and a defect or a malicious use pattern can suddenly turn a benign logging command into a fountain of a disastrous message flood. Thus, it is usually a good idea to adopt logging rate limiters, especially for sophisticated long-running programs.

On the other hand, a log rate limiter that is too restrictive can hinder debugging as well. Since, it is generally unpredictable which messages a limiter will drop, which can easily break the logging context that is critical for debugging. Thus, it is recommended to have at least one way to turn the limiter off.


# Conclusion

As usually the first thing to be checked when something goes wrong with a program, logging is a powerful tool for debugging. However, logging itself, sometimes, can become a source of problems.

In today's post, we have talked about the common ways of reducing messages get logged, with an emphasize on how to integrate protective rate limiter algorithms with a logging framework. A logging rate limiter can help to prevent the logging system from causing resource starvation which is the result of a sudden logging requests burst. For which reason, a logging rate limiter can be seen as a critical part of a robust program.

I hope you have found this post useful, if so, you may want to check out [other articles which also belong to the PDS series](/pds). <!-- JEKYLL_RELATIVE_URL_CHECK_SKIP_LINE -->


# References

1. [Token bucket](https://en.wikipedia.org/wiki/Token_bucket)
1. [Leaky bucket](https://en.wikipedia.org/wiki/Leaky_bucket)
1. [C++11: std::threads managed by a designated class](https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/) by Rafał Cieślak
1. [Rate Limiter — Sliding Window Log](https://medium.com/@avocadi/rate-limiter-sliding-window-log-44acf1b411b9)
1. [System Design Interview - An insider's guide (#ad)](https://www.amazon.com/) by Alex Xu
