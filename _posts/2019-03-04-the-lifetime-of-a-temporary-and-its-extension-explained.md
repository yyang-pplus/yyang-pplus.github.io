---
title: "The Lifetime of A Temporary and Its Extension: Explained"
date: "2019-03-04"
categories:
  - "cpp"
tags:
  - "c++"
  - "c11"
  - "class"
  - "cmake"
  - "gcc"
  - "language-lawyer"
  - "lifetime"
  - "object"
  - "programming"
  - "reference"
  - "software"
---

You probably already known that, in C++, every temporary object has a lifetime, which begins when it is created and ends when it is destroyed automatically. However, do you know you can extend temporary objects' lifetime with a reference? In this post, we are going to discuss the lifetime of temporary objects, how to extend their lifetime with a reference, and also the exceptions when that extension cannot be applied.


# Sample Code

A piece of sample code is worth a thousand words. This post contains sample code to help you better understand the somewhat abstruse wording of the C++ standard. The full source is located on [my GitHub]({{site.github.repository_url}}/tree/master/_includes/src/2019-03-04-lifetime-of-temporary). You can compile it with CMake or directly with any c++ compilers of your choice. Following is the command I used to compile it with GCC.

```bash
g++ a-temporary.cpp -o a-temporary -std=c++14 -g
```

I have tested the sample code using **GCC 7.3.0** on **Ubuntu 18.04.1 LTS** with **C++11** and **C++14**. They all give the same result.


# Helper Function

Before we get started, let us first look at the helper function, or macro, I use to better present the outcomes.

```cpp
{% include src/2019-03-04-lifetime-of-temporary/trace.hpp %}
```

**TRACE\_FUNCTION\_CALL()** macro is just a convenient way to show the name of the current function.


# Lifetime of a Temporary

> Normally, a temporary object lasts only until the end of the full expression in which it appears.[<sup>\[1\]</sup>](#references)

```cpp
{% include src/2019-03-04-lifetime-of-temporary/base.hpp %}
```

```cpp
{% include src/2019-03-04-lifetime-of-temporary/a-temporary.cpp %}
```

The output looks like this:

```bash
$ ./a-temporary
Base::Base()
Base::~Base()
int main()
```

As you can see, it first constructs the temporary **Base** object. After that, the temporary object is destructed immediately, before reaching the line that prints out the name of the **main()** function.


# Extend the Lifetime of a Temporary

> Whenever a reference is bound to a temporary or to a subobject thereof, the lifetime of the temporary is extended to match the lifetime of the reference.[<sup>\[2\]</sup>](#references)

With the same **Base** struct, but this time we bind the temporary object to a const lvalue reference:

```cpp
{% include src/2019-03-04-lifetime-of-temporary/const-lvalue-reference.cpp %}
```

Now the output becomes:

```bash
$ ./const-lvalue-reference
Base::Base()
int main()
Base::~Base()
```

The difference is pretty clear, when you compare it with [the previous example](#lifetime-of-a-temporary). This time, after binding to a const lvalue reference, the temporary object is destructed after printing the name of **main()**.

Of course, this lifetime extension of a temporary object, also works with rvalue reference.

```cpp
{% include src/2019-03-04-lifetime-of-temporary/rvalue-reference.cpp %}
```

This example gives the same result as the const lvalue reference one.

An interesting aspect of this feature is that when the reference does go out of scope, the same destructor that would be called for the temporary object will get called, even if it is bound to a reference to its base class type.[<sup>\[1\]</sup>](#references)

```cpp
{% include src/2019-03-04-lifetime-of-temporary/derived.hpp %}
```

```cpp
{% include src/2019-03-04-lifetime-of-temporary/derived.cpp %}
```

The output:

```bash
$ ./derived
Base::Base()
Derived::Derived()
int main()
Derived::~Derived()
Base::~Base()
```

It has to be pointed out that neither struct **Base** or **Derived** has a virtual desctructor. And when the lifetime of the temporary Derived object, which binds to a **Base** reference, ends, the right destructor **Derived::~Derived()** gets called.


# Exceptions

There are some exceptions to this rule where the lifetime of a temporary object cannot be extended.


## Exception 1

> a temporary bound to a return value of a function in a return statement is not extended: it is destroyed immediately at the end of the return expression. Such function always returns a dangling reference.[<sup>\[2\]</sup>](#references)

```cpp
{% include src/2019-03-04-lifetime-of-temporary/exception-1.cpp %}
```

GCC will also warn you for this: `warning: returning reference to temporary [-Wreturn-local-addr]`.

Output of this example:

```bash
$ ./exception-1
const Base& FunctionException1()
Base::Base()
Base::~Base()
int main()
```


## Exception 2

> a temporary bound to a reference member in a constructor initializer list persists only until the constructor exits, not as long as the object exists. (note: such initialization is ill-formed as of DR 1696).[<sup>\[2\]</sup>](#references)

It also has a mark says "until C++14". But this doesn't mean this exception is gone. Rather it has been replaced with an improved version and moved to other places. Please refer to [CWG 1696](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1696) for more details. One case of this exception is [N4800](https://github.com/cplusplus/draft/blob/master/papers/n4800.pdf) § 10.9.2 \[class.base.init\] paragraph 8:

> A temporary expression bound to a reference member in a mem-initializer is ill-formed.

```cpp
{% include src/2019-03-04-lifetime-of-temporary/exception-2.cpp %}
```

The result should be:

```bash
$ ./exception-2
Base::Base()
Derived::Derived()
DerivedWrapper::DerivedWrapper()
Derived::~Derived()
Base::~Base()
int main()
DerivedWrapper::~DerivedWrapper()
```

Apparently, if the lifetime of the temporary **Derived** object were extended, its destructor should be called after **DerivedWrapper::~DerivedWrapper()** get called. GCC doesn't warn you by default. But you can see the warning message if you compile the code with the **\-Wextra** flag to turn on extra warnings. The warning message may looks like: `warning: a temporary bound to ‘DerivedWrapper::d’ only persists until the constructor exits [-Wextra]`.


## Exception 3

> a temporary bound to a reference parameter in a function call exists until the end of the full expression containing that function call: if the function returns a reference, which outlives the full expression, it becomes a dangling reference.[<sup>\[2\]</sup>](#references)

```cpp
{% include src/2019-03-04-lifetime-of-temporary/exception-3.cpp %}
```

Here, the output:

```bash
$ ./exception-3
Base::Base()
const Base& FunctionException3(const Base&)
Base::~Base()
int main()
```

The reference parameter does extend the lifetime of the temporary **Base** object until the end of the function **FunctionException3()**, but not after the function returns. I don't think GCC has a warning for this exception, so if you know how to trigger the warning, please leave comments.


## Exception 4

> a temporary bound to a reference in the initializer used in a new-expression exists until the end of the full expression containing that new-expression, not as long as the initialized object. If the initialized object outlives the full expression, its reference member becomes a dangling reference.[<sup>\[2\]</sup>](#references)

One example of this exception is:

```cpp
{% include src/2019-03-04-lifetime-of-temporary/base-wrapper.hpp %}
```

```cpp
{% include src/2019-03-04-lifetime-of-temporary/exception-4.cpp %}
```

And, the output on my machine:

```bash
$ ./exception-4
Base::Base()
Base::~Base()
int main()
```

Notice, the **Base** destructor get called before printing the name of **main()**, and well before `delete w;`.


# A Special Case

The following case may be considered quite special, or not, when you compare it with the [Exception 4](#exception-4).

```cpp
{% include src/2019-03-04-lifetime-of-temporary/special-case.cpp %}
```

The result shows:

```bash
$ ./special-case
Base::Base()
int main()
Base::~Base()
```

Basically, this means that you **CAN** extend the lifetime of a temporary object by binding it to a reference that is a member of an object.


# Summary

In general, the lifetime of a temporary cannot be further extended by "passing it on": a second reference, initialized from the reference to which the temporary was bound, does not affect its lifetime.[<sup>\[2\]</sup>](#references) It is also worth to point out that binding a temporary array to an instance of **std::initializer_list** works much like binding a temporary object to a reference.[<sup>\[3\]</sup>](#references)


# References

1. [GotW #88: A Candidate For the “Most Important const”](https://herbsutter.com/2008/01/01/gotw-88-a-candidate-for-the-most-important-const/) by Herb Sutter.
1. [Reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization)
1. [The cost of std::initializer\_list](https://akrzemi1.wordpress.com/2016/07/07/the-cost-of-stdinitializer_list/) by Andrzej Krzemieński.
1. [Stack Overflow: Aggregate reference member and temporary lifetime](https://stackoverflow.com/questions/35313292/aggregate-reference-member-and-temporary-lifetime)
