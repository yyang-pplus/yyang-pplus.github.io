---
title: "PDS: Stack Backtrace"
date: "2021-04-08"
categories:
  - "cpp"
tags:
  - "backtrace"
  - "c++"
  - "core"
  - "debugging"
  - "engineering"
  - "gdb"
  - "practical-debugging-skills"
  - "programming"
  - "pstack"
  - "SIGSEGV"
  - "software"
---


Stack backtraces play a crucial role in programming, particularly in debugging and troubleshooting scenarios. Backtraces are essential tools for diagnosing a wide range of issues. Utilizing backtrace effectively when debugging is a core programming skill every developer should master. In this post, we'd like to discuss this skill in detail.


# What is a stack backtrace

A **stack backtrace** also known as **stack traceback**, **stack trace**, or simply **backtrace**.[<sup>\[1\]</sup>](#references) It is a list of the currently active function calls in a thread of a program.[<sup>\[2\]</sup>](#references) It is a summary of how a program got where it is.[<sup>\[3:§8.2\]</sup>](#references) A backtrace at the right point of a program can provide valuable insight into the program's status. In particular, a backtrace at the point of an error can be extremely helpful for debugging, as it can provide valuable insight into the context in which the error occurred, thus helping a developer to narrow down the scope of the issue quickly. Another way of my favorites to utilize a backtrace is when I work on a complex unfamiliar program and I need a quick way to figure out which execution path takes the program to a certain point to help me better understand the flow of the program.


# How to get a backtrace

The usual way to inspect a backtrace of a program is to use an external debugger program.[<sup>\[2\]</sup>](#references) Among those debuggers, **the GNU Debugger (GDB)** is the most popular one for many Unix-like systems.

## GDB

To retrieve a backtrace, you first need to invoke GDB. You can start GDB and attach it to an already running program with the `--pid` option:

```bash
sudo gdb ${program} --pid ${pid}
```

Here, `${program}` is the path to the executable of the running program. `${pid}` represents the process ID of the running program.

Alternatively, to debug a crashed program, you can start GDB with both an executable program and a core file specified by the `--core` option:

```bash
gdb ${program} --core ${core}
```

In the above command, `${program}` is the path to the executable you want to debug. `${core}` is the path to the core dump file that got created when the program crashed.

If you forget which executable the core file is from, you can find it out with the `file` command, for example, the bash command below shows that the sample core file `core.894` is from `containerd`:

```bash
$ file core.894
core.894: ELF 64-bit LSB core file, x86-64, version 1 (SYSV), SVR4-style, from '/usr/bin/containerd'
```

After GDB has been invoked, and when you get a prompt, you can print a backtrace of the entire stack by using the `backtrace` GDB command, or its aliases `bt`, `where`, or `info stack`. The `backtrace` command supports many options, one of the most useful options, especially during a non-interactive debug session to collect as much information as you can, is `full` or `-full`, which let the `backtrace` command prints the values of the local variables as well. For more information about other options, please refer to the GDB manual, or use the `help backtrace` GDB command.[<sup>\[3:§8.2\]</sup>](#references)

In a multi-threaded program, GDB by default shows the backtrace only for the current thread. To display the backtrace for all the threads, use the following GDB command:[<sup>\[3:§8.2\]</sup>](#references)

```gdb
(gdb) thread apply all backtrace
```

Here, `(gdb)` at the beginning of the above line means that the command following it should be run at the GDB prompt.

Put it all together, below is an example one-liner shell command to print the backtrace for all the threads of `containerd`.

```bash
sudo gdb --batch -ex 'thread apply all backtrace full' $(which containerd) $(pidof containerd)
```

This command runs GDB in batch mode. Batch mode disables pagination, sets unlimited terminal width and height, and acts as if `set confirm off` were in effect. The introductory and copyright messages are also suppressed in batch mode, as if the `-quiet` or `-silent` option is supplied.[<sup>\[3:§2.1.2\]</sup>](#references)

## pstack

Another debugger tool of my favorites is `pstack`, when I only need to print the backtrace of a running process. For example, if you want to peek at what is going on inside `containerd`, you can simply do it with the following shell command:

```bash
sudo pstack $(pgrep containerd)
```

Of course, GDB is more versatile compares to `pstack`, but `pstack` wins for its simplicity at the right situations. In fact, some versions of `pstack` are just implemented as shell script wrappers around GDB. Also note, `gstack` is a common equivalent tool to `pstack`, when it is not available.


# How to analyze a backtrace

Backtraces can be used for diagnosing a wide range of runtime issues, including invalid pointer dereferences, memory corruptions, out-of-bound accesses, infinite recursions, assertion failures, and many others. In this section, I am going to show you an example of what a typical backtrace analysis debugging session may look like.

Let us consider the following contrived example code:

```cpp
{% include src/2021-04-08-pds-stack-backtrace/dereference-null-pointer.cpp %}
```

Of course, this silly code itself does not have much meanings. It is just used as a reference point to guide our discussion. Compiling and running this sample code, would likely result in a crash and a core file been dumped. Assuming it does, then we can get a backtrace using GDB with the method described in the previous section. The backtrace may look as follows, with unrelated information chopped off:

```gdb
Program terminated with signal SIGSEGV, Segmentation fault.
...
#0  half (pointer=0x0) at dereference-null-pointer.cpp:2
#1  stepThree () at dereference-null-pointer.cpp:21
#2  process (config=2) at dereference-null-pointer.cpp:31
#3  main () at dereference-null-pointer.cpp:36
```

The first thing to analyze a backtrace is identifying the error message associated with it. In our case, that is the first line of the above output, which basically tells us the program crashed due to a segmentation fault. This can give us a high level idea of what issue we are facing and sometimes a valuable hint to the root cause. This step can be extended to any application level error messages those are related to the backtrace as well.

With the relevant error message in mind, now we can look at the actual backtrace. A backtrace is typically presented as lines of function calls, or stack frames to be more technical. Each line in the backtrace represents a function call, starting with the most recent function call that led to the error, following by its caller, and going back to the entry point of the program. Each line in the backtrace usually shows the function names, source code file names, line numbers, and sometimes additional context such as arguments to the function or program counters.[<sup>\[3:§8.2\]</sup>](#references) This information helps us to pinpoint the exact location in the source code where each function was called from and in what order. This is particularly important if there exists multiple code paths that could lead to the same location in the source, as it can save us a lot of time by avoid looking at the code that are irrelevant to the issue. In our example above, it has four function calls that are currently active in the program, which are numbered from 0 to 3. It starts with `half` as the most recent function call, followed by `stepThree` as its caller, and going all the way back to the `main` function.

When analyzing a backtrace, I usually start from the top (zero frame) of the backtrace, as the top typically represents the function where the failure occurred, although this is not necessarily where the failure caused.[<sup>\[1\]</sup>](#references) The top in our example has `half` as the function name, `dereference-null-pointer.cpp` as the source file name, followed by `2` as the line number. The specific source file name and line number indicate the exact position in the code where the error occurred. That is the following line in our example:

```cpp
*pointer /= 2;
```

By reviewing the relevant code, we know that `pointer` is a parameter to the `half()`. The backtrace also shows that the value of `pointer` is `0x0` (null) when it was passed into the function and was not changed afterward. This is directly related to the error message we have seen before, as dereferencing a null pointer is an undefined behavior in C++, which may cause a segmentation fault. Thus, it is reasonable to conclude that the direct cause of our issue is dereferencing a null pointer.

Sometimes, the direct cause is also the root cause of the issue, but when it is not the case, we will have to investigate further to find out the root cause, which for our issue is to find out where `pointer` came from and how it can be a null pointer. In order to do so, we need to walk backward through the backtrace, examining each function call and its associated code. According to our example backtrace, we knew that `stepOne()` called `half()` at line 21 of the same source file. Here is a copy of the same line:

```cpp
half(three);
```

As we can see, `stepThree()` passed `three` as the argument to `half()`. After checking the associated code, we find that `three` came from the same function and it was never pointed to a meaningful address. That is the root cause we are looking for.

Of course, issues we are going to face in the real world will unlikely be that straight forward as the example we just showed, but the underlying concepts are essentially the same.


# Conclusion

When troubleshooting an issue of a program, developers often wonder what is going on inside the program when the error happened. The Backtrace is one of the best tools that can answer this kind of questions. Thus, being familiar with backtraces is essential for improving one's debugging skills.

In today's post, we have discussed about what a backtrace is, and its role in debugging. We have also showed how to get the right backtraces with the help of external debuggers, and how to analyze the retrieved backtraces to find out the root causes of the issues more effectively.

If you found this post interesting, you may also want to check out [the other articles of my PDS series](/pds). <!-- JEKYLL_RELATIVE_URL_CHECK_SKIP_LINE -->


# References

1. [Stack trace](https://en.wikipedia.org/wiki/Stack_trace)
1. [The GNU C Library. 34.1 Backtraces](https://www.gnu.org/software/libc/manual/html_node/Backtraces.html)
1. [Debugging with GDB](https://sourceware.org/gdb/onlinedocs/gdb/index.html)
