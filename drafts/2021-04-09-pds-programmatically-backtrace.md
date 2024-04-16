---
title: "PDS: Programmatically Backtrace"
date: "2021-04-09"
categories:
  - "cpp"
tags:
  - "c++"
  - "debugging"
  - "engineering"
  - "GNU"
  - "practical-debugging-skills"
  - "programming"
  - "software"
---


https://github.com/google/glog?tab=readme-ov-file#failure-signal-handler


* what we mean by getting backtrace programmatically
* why/when to get programmatically backtrace
* what to consider when choosing the right library for getting backtrace
  https://stackoverflow.com/questions/6934659/how-to-make-backtrace-backtrace-symbols-print-the-function-names
  https://stackoverflow.com/questions/3899870/how-to-print-a-stack-trace-whenever-a-certain-function-is-called
  https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-program-crashes
  https://www.qnx.com/developers/docs/6.5.0SP1.update/com.qnx.doc.neutrino_technotes/backtrace.html
  https://stacktrace.sourceforge.net/
  https://www.linuxjournal.com/article/6391
  https://www.boost.org/doc/libs/1_85_0/doc/html/stacktrace.html
* basic form
* in terminate handler
* in exception handling
* in signal handler (signal safety)

* for multithreaded program
  https://code.google.com/archive/p/google-coredumper/


# From inside a program

As you can see, using a debugger tool to get a backtrace is a fairly easy task. However, what if such a tool is not available where you need it. This kind of situations are common on restricted systems or on production servers which you do not have full controls. One solution to this problem is to let a program itself report a backtrace when certain events happen. Such events may include, but not limited to: when an exception is about to be thrown, or when the program is about to crash, or when the program receives certain signals.

Now, the question is how we can obtain a backtrace programmatically from within a program.

# Helper functions

# Boost.Stacktrace

**Boost.Stacktrace** library is a simple C++03 library which can be used to get information about call sequence.[<sup>\[4\]</sup>](#references) When configured properly, it can show line numbers and support C++ name demangling well. One concern people may have is that **Boost.Stacktrace** is not signal safe on all the platforms, so it is generally recommend to avoid using **Boost.Stacktrace** inside signal handlers. The following sample code is an example of using **Boost.Stacktrace** in a terminate handler.

```cpp
{% include src/2021-04-08-pds-stack-backtrace/boost-stacktrace-on-terminate.cpp %}
```

There are a few things about that example I would like to point out. First, the `on_terminate` function is registered as the desired terminate handler during the initialization of the global variable `installed`. This is a technique to set a user defined terminate handler before `main()` starts to reduce the risk of surprises. Second, inside the handler, `stacktrace()` is the default constructor to actually get and store the current function call sequence information into a temporary variable, then that information is immediately send to the standard error stream. Third, the entire handler is protected by a mutex to prevent it from being called concurrently by different threads in a multi-threaded environment. Last, `on_terminate()` swallows all potential exceptions, since a terminate handler should never throw.[<sup>\[5\]</sup>](#references)

**Boost.Stacktrace** can be built in two modes. By default, it is a header-only library, but you can also disable header-only build and require linking by defining the `BOOST_STACKTRACE_LINK` link macro, as we have already seen in the previous example.[<sup>\[4\]</sup>](#references) I will show you how to use the header-only mode shortly in the next example.

Assertions are an useful debugging tool to help developers to quickly locate a problem, but the default facility provided by the C++ standard for making an assertion often does **not** contain enough information to pinpoint the problem. One way to improve the assertions is to add backtrace to the assertion messages. The following simple example shows how we can do that with the help of **Boost.Stacktrace**.

```cpp
{% include src/2021-04-08-pds-stack-backtrace/boost-stacktrace-on-assert.hpp %}
```

The macro `BOOST_STACKTRACE_USE_BACKTRACE` here means we want to use the **libbacktrace** version of the implementation.


# C++23

Unfortunately, in C++ prior to C++23, we do not have native support for retrieving a backtrace. This has been changed since [P0881R7][https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0881r7.html) got approved for C++23. Since C++23, there will be `<stacktrace>`. This will, of course, be the most portable way to get a backtrace programmatically. Its design is based on the **Boost.Stacktrace** library, thus we can expect it to share most of the same features and properties as **Boost.Stacktrace**. For this reason, the example code for `<stacktrace>` is omitted in this post.


# References

1. [Stack trace](https://en.wikipedia.org/wiki/Stack_trace)
1. [The GNU C Library. 34.1 Backtraces](https://www.gnu.org/software/libc/manual/html_node/Backtraces.html)
1. [Debugging with GDB](https://sourceware.org/gdb/onlinedocs/gdb/index.html)
1. [The Boost C++ Libraries BoostBook Documentation Subset: Chapter 35. Boost.Stacktrace 1.0](https://www.boost.org/doc/libs/1_83_0/doc/html/stacktrace.html)
1. [Possible Ways To End A C++ Program](https://yyang-pplus.github.io/cpp/2023/06/18/possible-ways-to-end-a-cpp-program.html)
