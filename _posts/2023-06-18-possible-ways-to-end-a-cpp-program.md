---
title: "Possible Ways To End A C++ Program"
date: "2023-06-18"
categories:
  - "cpp"
tags:
  - "abort"
  - "c++"
  - "exception"
  - "exit"
  - "handler"
  - "POSIX"
  - "programming"
  - "quick_exit"
  - "signal"
  - "software"
  - "terminate"
---


In C++, there exists many ways to end a program. You probably have heard most of them, but do you know how they are different from each other. It is important to be familiar with those termination mechanisms and their differences so that you can choose the right one at the right situation to make your programs better.


# The standard normal termination

## Return from the main()

A program terminates, when the `main` function reaches its end or encounters a `return` statement. This is, of course, the most common and the most clean way to end a C++ program, for normal program flows. Behind the scene, after returning from the `main` function, the program executes `std::exit()` with the return value of the `main` function as its argument.[<sup>\[1\]</sup>](#references)

However, sometimes, it may be painful to reach back to the `main` function, if you are deep in the call stack. That is one of the reasons why you need to be familiar with the other possible ways to end a program.

## Exceptions and stack unwinding

As you will see later in this article, the other termination mechanisms usually do **not** perform any sort of stack unwinding to destruct local variables. It nature to ask, **why not?**

One of the reasons to avoid stack unwinding is for debugging. When an error occurs in a program, it is often helpful to get a backtrace, or generate a core dump first, at the point of the error. That usually means the status of the program's call stack has to be recorded before any stack unwinding is done, so that it will contain more information than if it were recorded after unwinding. In the case of an exceptional situation, such as a crash, it usually means no stack unwinding can be performed in order to preserve the information. And that is one of the reasons why the C++ standard allows stack unwinding to be skipped at certain situations.

However, sometimes, the destructors of your local variables may perform critical cleanup tasks, such as release critical system resources. Note, here the critical system resources usually do **not** include regular resources like memory, as typically the system will release all memory, along with cleaning up many other things, after a program ends. In case you want to ensure the destructors of those local variables get called properly during stack unwinding, it is better to wrap the entire body of the `main()` with exception handlers that catch all exceptions. For example:[<sup>\[10\]</sup>](#references)

```cpp
{% include src/2023-06-18-possible-ways-to-end-a-cpp-program/catch-all-main.cpp %}
```

Then, in the right places, use `throw ExitException(exit_code);`, instead of `std::exit(exit_code);`. Of course, with this technique, the exact point where an exception was thrown is generally lost.


# Normal terminations

Beside the standard normal termination, returning from the `main()`, there are three other ways to exit a program normally. Here, as I will explain shortly, normal does **not** mean clean.

## std::exit() and std::atexit()

Defined in the `<cstdlib>` header, the built-in `std::exit` function can be used to cause normal program termination to occur from any place of the code. It is declared as follows:[<sup>\[1\]</sup>](#references)

```cpp
[[noreturn]] void exit(int exit_code);
```

Passing `0` or `EXIT_SUCCESS` as the argument to `std::exit()` indicates successful termination, while `EXIT_FAILURE` indicates unsuccessful termination. `std::exit()` does mainly three things before it ends a program:[<sup>\[1\]</sup>](#references)

* Executes the destructors of objects with static storage duration and the functions registered with `std::atexit()`. Since C++11, it also calls the destructors of objects with thread local storage duration that are associated with the current thread. If any of these functions or destructors throw an exception, `std::terminate()` is called.
* Flushes and closes all C streams.
* Removes files created by `std::tmpfile()`.

The difference between calling `std::exit()` and returning from the `main()` is that `std::exit()` does **not** perform any sort of stack unwinding; it means that `std::exit()` does **not** call the destructors of variables with automatic storage duration.[<sup>\[1\]</sup>](#references)

The `std::atexit` function can be used to register functions to be called on normal program termination via calling `std::exit()` or returning from the `main` function. The same function may be registered more than once. The registered functions get called in a **last in, first out (LIFO)** fashion.[<sup>\[2\]</sup>](#references) This mechanism is often used for performing cleanup tasks before a program terminates. In fact, some compilers use this mechanism to call the destructors of global objects.[<sup>\[10\]</sup>](#references)

## std::quick_exit() and std::at_quick_exit()

Also defined in the `<cstdlib>` header, the `std::quick_exit` function is a new built-in function in C++11 which can be used to cause normal program termination to occur without completely cleaning the resources. It is declared as follows:[<sup>\[3\]</sup>](#references)

```cpp
[[noreturn]] void quick_exit(int exit_code) noexcept;
```

Before `std::quick_exit()` ends a program, the only thing it does is to call functions passed to `std::at_quick_exit()` in the reverse order of their registration, and it performs no other cleanup. If an exception tries to propagate out of any of the registered functions, `std::terminate()` is called. After calling the registered functions, `std::quick_exit()` calls `std::_Exit(exit_code)` to terminate the program.[<sup>\[3\]</sup>](#references) In particular, when comparing to return from the `main()`, `std::quick_exit()` does **not** perform any stack unwinding, or does **not** clean up objects with static storage duration, or does **not** call functions passed to `std::atexit()`.

## std::_Exit()

Defined in the `<cstdlib>` header as well, the new C++11 `std::_Exit` function causes normal program termination to occur without completely cleaning the resources. It is declared as follows:[<sup>\[4\]</sup>](#references)

```cpp
[[noreturn]] void _Exit(int exit_code) noexcept;
```

`std::_Exit()` performs no cleanup of resources before it ends a program. More specifically, when comparing to return from the `main()`, `std::_Exit()` does **not** perform any stack unwinding, does **not** clean up objects with static storage duration, or does **not** call atexit handlers passed to `std::at_quick_exit()` or `std::atexit()`. Whether open resources such as files are closed is implementation defined.[<sup>\[4\]</sup>](#references)

## Compare normal terminations

One question that naturally comes up is, **why we have so many ways to terminate a program normally, are they all really necessary?**

The main difference between `std::exit()` and the other two is that `std::exit()` executes the destructors of objects with static storage duration. To safely destruct static-duration objects requires that all users threads have been canceled, if necessary, and finished cleanly. However, the problem is that for various reasons a clean cancellation of threads may be difficult, or even impossible, to achieve, especially for legacy programs. In that case, it would be better to never execute static destructors.[<sup>\[5\]</sup>](#references) Thus, you may need to use `std::quick_exit()` or `std::_Exit()` instead to end a multi-threaded program. Also, avoid calling `std::exit()` in a destructor, especially a global object's destructor, as that may cause an infinite recursion.[<sup>\[16:§15.4.3\]</sup>](#references)

As for `std::quick_exit()` and `std::_Exit()`, the main difference between them is that `std::quick_exit()` calls functions passed to `std::at_quick_exit()`. Similar to functions passed to `std::atexit()`, those functions registered with `std::at_quick_exit()` can be used to perform critical cleanup tasks, such as flush critical information to stable storage.[<sup>\[5\]</sup>](#references) Therefore, `std::quick_exit()` should be preferred over `std::_Exit()` if such cleanup tasks are important for your program.


# Abnormal terminations

Beside the normal terminations, there are two other ways to terminate a program abnormally in case of an emergency. Here, the emergency usually means when the program will not be able to recover from an error condition.

## std::terminate() and terminate-handlers

Defined in header `<exception>`, the `std::terminate` function can be used to end a program abruptly. It is declared as follows since C++11:[<sup>\[6\]</sup>](#references)

```cpp
[[noreturn]] void terminate() noexcept;
```

`std::terminate()` can be called by the C++ runtime for various reasons or it can also be called directly from the program. In any case, `std::terminate()` simply calls the currently installed terminate handler. The default terminate handler calls `std::abort()`, which leads to abnormal program termination by default. In other words, the default behavior of `std::terminate()` is just simply terminate the program with no cleanup. In particular, if `std::terminate()` is called due to an exception is thrown and not caught, the C++ standard allows stack unwinding to be skipped in such case.[<sup>\[6\]</sup>](#references) Moreover, do **not** confuse the currently installed handler with the most recently installed handler, there are situations where they may **not** necessarily be the same one. Thus, in order to avoid surprises it is better to install the desired handler as soon as possible (ASAP) in the program and not change it afterwards. Here, ASAP means during the initialization of globals and before `main()` starts.[<sup>\[8\]</sup>](#references)

You can replace the global terminate handler function using the `std::set_terminate` function. Noticeably, unlike atexit handlers registered with `std::atexit()` or `std::at_quick_exit()`, there is only one single terminate handler installed at any given time.[<sup>\[7\]</sup>](#references) In a multi-threaded environment, that also means the same terminate handler may be executed concurrently by different threads. Thus, you need to take extra care to make sure your terminate handler is thread-safe and some tasks that can only be performed once will not be executed multiple times.[<sup>\[8\]</sup>](#references)

Since C++11, the `std::terminate` function is declared with the `[[noreturn]]` attribute, which means that the terminate handler shall terminate execution of the program without returning to the caller, otherwise the behavior is undefined.[<sup>\[7\]</sup>](#references) So typically in a termination handler we want to do something and then call `std::abort()` or `std::_Exit()`.[<sup>\[8\]</sup>](#references) This is another difference between a terminate handler and an atexit handler.

Similar to `std::at_quick_exit()`, `std::terminate()` is declared as `noexcept` in C++11. Whenever an exception is thrown from a `noexcept` function, `std::terminate()` is called and it is implementation-defined whether any stack unwinding is done in this case.[<sup>\[6\]</sup>](#references) For a terminate handler, that also means if it throws, `std::terminate()` is called recursively.

Also note, unlike the normal termination mechanisms, the abnormal termination mechanisms, like `std::terminate()`, are designed to handle irrecoverable errors when the usual error recovery strategy implemented by the exception handling mechanism has failed. In such an exceptional situation, in general, there are **not** many things we can safely rely on. For one, `std::terminate()` may be called before global variables have been initialized or after they have been destroyed, therefore we cannot rely on any globals. The only exception are the eight C++ standard stream global objects: `cin`, `cout`, `cerr`, `clog`, `wcin`, `wcout`, `wcerr`, and `wclog`. They are guaranteed to be never destroyed.[<sup>\[8\]</sup>](#references) Technically, even those stream objects are not entirely safe to use, as they may have already been corrupted in an extreme situation.[<sup>\[16:§13.5.2.5\]</sup>](#references) For another, when `std::terminate()` gets called, the program is usually in a highly unstable state, therefore we should avoid using any functions that explicitly rely on dynamic memory allocation inside a terminate handler.

For those reasons, the things you can do within a terminate handler are rather limited.[<sup>\[8\]</sup>](#references) A terminate handler is typically used to perform critical cleanup tasks or log information for debugging. When you do so, it is often best to keep it as simple and robust as possible. One good example of a terminate handler is to inspect the uncaught exception if `std::terminate()` is called due to a thrown exception by using `std::current_exception()` and `std::rethrow_exception()`, like this:[<sup>\[8\]</sup>](#references)

```cpp
{% include src/2023-06-18-possible-ways-to-end-a-cpp-program/catch-current-exception-on-terminate.hpp %}
```

For more information about the good things you can do with `std::terminate()`, please refer to the reference material [\[8\]](#references).

## std::abort() and signal handler

The `std::abort` function is from the `<cstdlib>` header. `std::abort()` simply raises an abnormal termination signal, `SIGABRT`. `SIGABRT`, in turn, causes abnormal program termination to occur, and a core to be dumped on some systems, by default. `std::abort()` is declared as follows since C++11:[<sup>\[9\]</sup>](#references)

```cpp
[[noreturn]] void abort() noexcept;
```

At first glance, `std::abort()` looks quite similar to `std::_Exit()`: it halts the program immediately without performing any cleanup; the only difference is that `std::abort()` does **not** take an exit code as its argument, and it always returns an exit code indicating unsuccessful execution to the host environment.[<sup>\[8\]</sup>](#references) More specifically, `std::abort()` does **not** perform any stack unwinding, does **not** clean up objects with static storage duration, or does **not** call atexit handlers passed to `std::at_quick_exit()` or `std::atexit()`. Whether open resources such as files are closed is implementation defined.[<sup>\[9\]</sup>](#references) However, as an abnormal termination mechanism, `std::abort()` should only be used in an extreme situation.

The `std::signal` function, defined in `<csignal>` header, can be used to set a user-defined function as the handler for a signal, such as `SIGABRT`. A signal handler is automatically invoked when the corresponding signal is delivered. Similar to a terminate handler, a signal handler is a per-process attribute: the signal handler of a particular signal is the same for all threads of the program.[<sup>\[11\]</sup>](#references) The difference between a signal handler and a terminate handler is that a signal handler is part of the C signal handling mechanism, while a terminate handler is part of the C++ exception handling mechanism. On one hand, a signal handler is more powerful, as it can be used to cover more abnormal situations where `std::terminate()` is not called but a signal is raised. For example, when an assertion failed or a bus error encountered. On the other hand, what we can do with a signal handler is more restricted; in other words, the signal safety rules are more restrictive than terminate handler rules we have talked about in the previous section. In particular, a signal handler using exception handling is likely to have problems.


# std::signal and more about signal handler

In the previous section, we have briefly mentioned `SIGABRT` and signal handlers. In this section, for the sake of completeness, we will talk about other signals that can end a program. Signal handling itself seems deceivingly simple, but actually is not. For this reason, we will not be able to cover every aspect of signal handling in this post, instead we will try to limit the scope to only the parts that are relevant to the main topic of this article.

In short, signals are software interrupts delivered to a process when a event happens. Signals can be posted to a process when the system detects a software event, such as using UNIX commands like `kill`, or calling library functions like `std::raise()`, or pressing special key bindings like **ctrl-c**. Signals can also come directly from the OS kernel when a hardware event such as a bus error or an illegal instruction is encountered.[<sup>\[12\]</sup>](#references) For most kinds of signals, the default action is to terminate the process. For certain kinds of signals that represent "harmless" events, the default action is to do nothing. Additionally, the signals that normally represent program errors have a special property: when one of these signals terminates the process, it also writes a core dump file which records the state of the process at the time of termination. In particular, the following signals are often used to tell a process to terminate:[<sup>\[13:§24.2.2\]</sup>](#references)

* `SIGTERM`: it is the normal way to politely ask a program to terminate.
* `SIGINT`: program interrupt; It is sent when the user types, normally, **ctrl-c**.
* `SIGQUIT`: it is sent when the user types, normally, **ctrl-\\**. It causes the process to terminate with a core dump. You can think of this as a program error condition "detected" by the user. Certain kinds of cleanups are best omitted in handling `SIGQUIT`.
* `SIGKILL`: it is used to cause immediate program termination. It cannot be handled or ignored, and is therefore always fatal. It is also not possible to block this signal.
* `SIGHUP`: hang-up; It is used to report that the user's terminal is disconnected or the controlling process is died.

Those signals have different names because they are used for slightly different purposes, and programs might want to handle them differently. The reason for handling these signals is usually to perform some critical cleanup tasks before actually terminating.[<sup>\[13:§24.2.2\]</sup>](#references)

As we have already mentioned, `std::signal()` can be used to change the action of a particular signal, except certain signals, such as `SIGKILL`, whose actions are always fixed. When implementing a signal handler that has a well-defined common convention of what a signal should do, it is usually better to follow the existing convention. A clean way to achieve that is to end the signal handler by specifying the default action for the signal that happened and then reraising it.[<sup>\[13:§24.2.2\]</sup>](#references) This is especially important when handling program error signals, like `SIGSEGV`, which should never return to the caller. However, you should avoid this technique for signals that are usually raised synchronously by calling `std::abort()` or `std::raise()`, as reraising a signal in such situations may cause undefined behavior.[<sup>\[15\]</sup>](#references)

Since a signal handler can be called asynchronously, at unpredictable times, POSIX requires a signal handler to be thread-safe, and may only call async-signal-safe functions.[<sup>\[14\]</sup>](#references) In additionally, use of certain C++ features inside signal handlers may cause undefined behavior.[<sup>\[15\]</sup>](#references) For those reasons, it is usually better to keep signal handlers away from heavy work. The best practice is to write a handler that does nothing but set an external variable that the program checks regularly, and leave all serious work to the program.[<sup>\[13:§24.4.6\]</sup>](#references) This is especially applicable to signals that may return to the caller, such as `SIGINT`.


# Summary

In today's post we have talked about various ways to terminate a program in C++. It is important to be familiar with those terminate mechanisms, as proper program terminations are essential for maintaining a well-behaved application. In general, you should prefer returning from the `main()` to end a program, whenever that is feasible. Use the other normal termination functions only if you have to and keep its use to the minimum. The problem with those normal termination methods is that, just like having multiple return statements in a function, you will end up having multiple exit points from a program, which may make the program hard to understand. The abnormal termination mechanisms should only be used in exceptional situations with caution. Also one should remember to handle terminate signals gracefully.


# References

1. [std::exit](https://en.cppreference.com/w/cpp/utility/program/exit)
1. [std::atexit](https://en.cppreference.com/w/cpp/utility/program/atexit)
1. [std::quick_exit](https://en.cppreference.com/w/cpp/utility/program/quick_exit)
1. [std::_Exit](https://en.cppreference.com/w/cpp/utility/program/_Exit)
1. [N2440: Abandoning a Process](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2440.htm) by Lawrence Crowl
1. [std::terminate](https://en.cppreference.com/w/cpp/error/terminate)
1. [std::terminate_handler](https://en.cppreference.com/w/cpp/error/terminate_handler)
1. [Using std::terminate](https://akrzemi1.wordpress.com/2011/10/05/using-stdterminate/) by Andrzej Krzemieński
1. [std::abort](https://en.cppreference.com/w/cpp/utility/program/abort)
1. [Who calls std::terminate?](https://akrzemi1.wordpress.com/2011/09/28/who-calls-stdterminate/) by Andrzej Krzemieński
1. [signal(7)](https://man7.org/linux/man-pages/man7/signal.7.html)
1. [IPC:Interrupts and Signals: \<signal.h\>](https://users.cs.cf.ac.uk/Dave.Marshall/C/node24.html) by Dave Marshall
1. [The GNU C Library: 24 Signal Handling](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)
1. [signal-safety(7)](https://man7.org/linux/man-pages/man7/signal-safety.7.html)
1. [std::signal](https://en.cppreference.com/w/cpp/utility/program/signal)
1. [The C++ Programming Language, 4th Edition (#ad)](https://www.amazon.com) by Bjarne Stroustrup
