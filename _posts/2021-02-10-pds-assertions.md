---
title: "PDS: Assertions"
date: "2021-02-10"
categories:
  - "cpp"
tags:
  - "assert"
  - "c"
  - "c++"
  - "concept"
  - "contract"
  - "engineering"
  - "practical-debugging-skills"
  - "programming"
  - "software"
---

In a programming language, such as C++, an assertion is a statement at a specific point in a program to help you to confirm that the statement it makes about your program is indeed true. Judicious use of assertions in a program may bring many benefits, especially, experience has shown that writing assertions while programming is one of the quickest and most effective ways to detect and correct bugs.[<sup>\[1\]</sup>](#references) Thus, having assertions at the right places of a program is one of the most commonly used preemptive techniques for debugging.


# Assertions in C++

In the standard C++ library, for now, we have basically two kinds of assertions: the compile-time assertion and the run-time assertion.

The **compile-time assertion**, or the `static_assert` declaration, performs compile-time assertion checking. The compile-time assertion takes the following form:

```cpp
static_assert(bool-constexpr, message)
```

Where:

* `bool-constexpr` is a constant expression represents a statement that needs to be checked when the code is compiled.
* `message` is a string literal that will appear as compiler error if `bool-constexpr` is false. The message string has become optional since C++17.[<sup>\[2\]</sup>](#references)

If `bool-constexpr` is well-formed and evaluates to `true`, `static_assert` declaration has no effect. Otherwise, a compile-time error is issued and the compilation fails.[<sup>\[2\]</sup>](#references)

The **run-time assertion**, or the `assert` preprocessor macro, which looks like a function call, performs run-time assertion checking. The run-time assertion has only one form:

```cpp
assert(condition)
```

Where:

* `condition` is an expression represents a condition that needs to be checked during the run time.

When run-time assertion is enabled, if `condition` evaluates to `false`, it displays implementation-specific diagnostic information on the standard error output and calls `std::abort`. Otherwise, the program continues normally.[<sup>\[3\]</sup>](#references)

Unlike `static_assert`, `assert` has no standardized interface to add an additional message to assert errors. A portable way to include one is to use a comma operator provided it has not been overloaded, or use `&&` with a string literal.[<sup>\[3\]</sup>](#references) For example:

```cpp
{% include src/2021-02-10-pds-assertions/show-assertion.cpp %}
```

Note, in the previous example, there is an extra pair of parentheses in the second `assert` statement, which uses a comma operator. That is because `assert` is a function-like macro, commas anywhere in condition that are not protected by parentheses are interpreted as macro argument separators. Such commas are often found in template argument lists and list-initialization.[<sup>\[3\]</sup>](#references) Also, attention should be payed to the order of the operands in the comma expression, since the result of a build-in comma operator always has the same type and value as the second operand. In our case, the condition we want to assert for should always come after the message string.

In general, compile-time checking should be preferred over run-time checking. Since, apparently, compile-time checking has no performance impact, and you certainly don't need to write error handlers for errors caught at compile time.[<sup>\[4 P.5\]</sup>](#references)


# When to use assertions

Assertions are mainly used during development and testing to catch errors early on. Here are a few places where assertions could be useful.

**Assertions can be used as a better form of comments to state your assumptions concerning a program's behavior,** where it is applicable. A common issue with comments is that comments are not updated as consistently as code. A wrong comment is worse than no comment, because it will lower one's confidence on the code itself, as it says, if the comment and the code disagree, both are likely to be wrong.[<sup>\[4 NL.2\]</sup>](#references) On the other hand, assumptions expressed in assertions can rarely be wrong, once a program has been tested thoroughly. One of such examples may look like this:

```cpp
{% include src/2021-02-10-pds-assertions/assertion-as-comment.hpp %}
```

As a variant to the previous situation, `assert(false)` **can be placed at any locations you assume will not be reached.**[<sup>\[1\]</sup>](#references) For example:

```cpp
{% include src/2021-02-10-pds-assertions/assert-false.hpp %}
```

Assertions used in this way not only improve the maintainability by acting as a better form of comments, but also guarantee the statements it makes about the effects of the code to be true.

Furthermore, **Some preconditions, postconditions, and invariants can be expressed as assertions.** The purpose of this kind of assertions is to check for programming errors, rather than to handle run-time errors that may occur during normal program execution. Like this:

```cpp
{% include src/2021-02-10-pds-assertions/assertion-as-precondition.hpp %}
```

Also note, most member functions have as a precondition that some class invariant holds. That invariant is established by a constructor and must be reestablished upon exit by every member function called from outside the class. You don’t need to mention it for each member function.[<sup>\[4 I.5\]</sup>](#references)

As a special case of the previous situation, `static_assert` **can also be used to check a class matches a concept:**[<sup>\[4 T.150\]</sup>](#references)

```cpp
{% include src/2021-02-10-pds-assertions/assert-on-concept.hpp %}
```

In additionally, **assertions can be used to provide more debugging information** to help the programmer to identify the source of the problem, especially, when there are no better means. Such as the following contrived function:

```cpp
{% include src/2021-02-10-pds-assertions/assert-before-crash.hpp %}
```

Without assertions, `Half` would crash with a segmentation fault, if given a null pointer. With an assertion, however, a nicer error message will be printed out before the program aborts. The error message is implementation-defined, but it usually contains the file name, line number, and the failed condition. Which makes it easier to track down the origin of the issue. Hence makes debugging more efficient.

Overall, the major benefit of assertions is to make debugging more effective, assuming the code path containing it is being properly tested.[<sup>\[5\]</sup>](#references) Moreover, assertions used in the right point can improve code quality and make it easier to maintain the code.


# Problems with assertions

There is no doubt that assertions are a powerful tool, however, just like everything else, assertions have their own downsides as well. Here are some situations where they should generally be avoided.

## Not an error-handling mechanism

The purpose of an assertion is not to handle an error, but it is to make it possible for the programmer to find and fix the issue faster.[<sup>\[5\]</sup>](#references) Especially, assertions should not be used to handle user input.

If that is the case, how should we express precondtions while still having a proper input validation? As we already mentioned, assertions are just one of the ways to state preconditions. Another common way is to use if-statements:

```cpp
{% include src/2021-02-10-pds-assertions/if-as-precondition.hpp %}
```

However, one potential problem with precondtions stated in if-statements is that this can make them hard to be distinguished from ordinary code.[<sup>\[4 I.6\]</sup>](#references) So, a more preferred way is to use `Expects()` macro from **guidelines support library**([GSL](https://github.com/Microsoft/GSL)) for expressing preconditions.

```cpp
{% include src/2021-02-10-pds-assertions/expects-as-precondition.hpp %}
```

Ideally, preconditions and postconditions should be part of the interface rather than part of the implementation, but we don’t yet have the language facilities to do that.[<sup>\[4 I.6\]</sup>](#references)

## Not being side effects free

As a rule, the expressions contained in run-time assertions should be free of side-effects.[<sup>\[1\]</sup>](#references) For reasons we will discuss shortly, assertions are generally disabled at run-time. Therefore, when the assertions with side-effects are stripped out the program's behavior changes in an unexpected way, which may introduce severe bugs that are only reproducible in certain build.[<sup>\[5\]</sup>](#references) Thus, it is not a good idea to write expressions in assertions that can affect any state of a program that is visible after the evaluation is complete. One exception to this rule is that assertions can modify state that is used only from within other assertions.[<sup>\[1\]</sup>](#references) However, side-effects do sometimes slip in, so the only reliable way to avoid these kind of issues is to test both builds, with and without assertions.[<sup>\[5\]</sup>](#references)

## Can impact performance

As already discussed, assertions should not affect the logic of a program, but run-time assertions can and often do impact the program's performance. Here, the performance includes the time to execute and the space. Simple assertions are relatively inexpensive, but more stringent checks, especially in assertion heavy code can measurably slow code down, sometimes severely so. Assertions also take up space, and not necessarily just for the code itself. As `assert` macros often embed the assertion message string in the source code. This can add up if assertions make up a substantial percentage of the code. For these reasons, assertions should be used sparsely and should be avoided in performance critical code, in order to prevent serious performance issues during development. Additionally, run-time assertions are typically disable in release build to produce a program that is both smaller and faster, assuming the program has been thoroughly tested and bug-fixed.[<sup>\[5\]</sup>](#references)

As the definition of the macro `assert` depends on another macro, `NDEBUG`, in order to disable run-time assertions, simply define `NDEBUG` as a macro name at the point in the source code where `<cassert>` or `<assert.h>` is included.[<sup>\[3\]</sup>](#references) As a matter of fact, in one source file, you can define and undefine `NDEBUG` multiple times, each time followed by `#include <cassert>`, to enable or disable the `assert` macro multiple times in the same source file.[<sup>\[6\]</sup>](#references)

Compiling the source code without run-time assertions can result in an appreciable difference in performance.[<sup>\[5\]</sup>](#references) On the other hand, compile-time assertions have no performance impact. Thus, it is usually unnecessary to disable them in production build.

## May have wrong semantics

The fact that run-time assertions abort in debug mode and check noting in production runs, may not always be what you want. On one hand, in debug mode, an assertion failure could elevate a minor issue into a bug which might block any more testing activities for a relatively long time, if the failure is only caught in a binary that is delivered to the QE team. On the other hand, in release mode, allow the program to continue in a wrongful state, where it should have been aborted by an assertion, could result in many bugs of different symptoms been reported that are essentially the same issue.[<sup>\[5\]</sup>](#references) Thus, wasting everyone's time. There are no really good solutions to this problem, just like everything else, learning to use assertions at the right places takes a good sense of intuition, experience and practices.


# Conclusion

In today's post, we talked about assertions as a useful tool to help the programmer to identify and fix bugs early in the development process, it can improve code quality and maintainability if used wisely. In short, assertions are not meant to be a replacement of error handling during normal program execution, rather the major role of assertions is to identify bugs faster, thus making software development more effective.

I hope you have found this post useful, if so, you may also want to check out [other articles which also belong to the PDS series](/pds). <!-- JEKYLL_RELATIVE_URL_CHECK_SKIP_LINE -->


# References

1. [Programming With Assertions](https://docs.oracle.com/javase/8/docs/technotes/guides/language/assert.html)
1. [static_assert declaration](https://en.cppreference.com/w/cpp/language/static_assert)
1. [assert](https://en.cppreference.com/w/cpp/error/assert)
1. [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) by Bjarne Stroustrup and Herb Sutter
1. [What Are Assertions](https://wiki.c2.com/?WhatAreAssertions)
1. [C++ In a Nutshell](https://www.oreilly.com/library/view/c-in-a/059600298X/re171.html) by Ray Lischner
