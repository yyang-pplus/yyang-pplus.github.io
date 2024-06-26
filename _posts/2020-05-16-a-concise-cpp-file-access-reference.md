---
title: "A concise C++ File Access Reference"
date: "2020-05-16"
categories:
  - "cpp"
tags:
  - "buffering"
  - "c"
  - "c++"
  - "filebuf"
  - "fstream"
  - "IO"
  - "Linux"
  - "POSIX"
  - "programming"
  - "software"
  - "stdio"
---

You are probably already familiar with accessing files in C++ using `fstream`, but do you know there are at least two other common ways to access files? In this post, I am going to present all three ways of doing file I/O in C++. The goal is to create a concise reference which summarizes and compares all those methods, so that people can choose between them quickly and easily, if needed.

# File stream classes

Defined in header `<fstream>`, the C++ standard library provides mainly four class templates for accessing files as streams:[<sup>\[1\]</sup>](#references)

```cpp
{% include src/2020-05-16-a-concise-cpp-file-access-reference/fstream-declaration.hpp %}
```

The class template `basic_ifstream<>` implements high-level input operations for reading from a file.<br>
The class template `basic_ofstream<>` implements high-level output operations for writing to a file.<br>
The class template `basic_fstream<>` implements high-level input/output operations for reading from and writing to a file.<br>
The class template `basic_filebuf<>` is used by the other file stream classes to perform the actual I/O of characters.

## FS: open/close

| fstream fs {}        | Construct a file stream object                                           |
| fstream fs {f, mode} | Construct a file stream object and open the **f**ile with given **mode** |
| fs.open(f, mode)     | Open the **f**ile with given **mode**                                    |
| fs.close()           | Close the associated file                                                |

One advantage of accessing files through file streams is the automatic management of file resources using the RAII pattern.[<sup>\[4:§15.9.1\]</sup>](#references)

There are a few predefined file stream object in header `<iostream>`:

| cin  | Associated with the standard input stream oriented to narrow characters                                          |
| cout | Associated with the standard output stream oriented to narrow characters                                         |
| clog | Usually the same as `cerr`, except it is not automatically flushed and not automatically **tie()**'d with `cout` |
| cerr | Associated with the standard error stream oriented to narrow characters                                          |

## FS: formatted I/O

| in \>\> x  | Read from an **in**put stream into **x** according to **x**'s type |
| out \<\< x | Insert **x** into the **out**put stream according to **x**'s type  |

The I/O operators mechanism is the key improvement over C's IO mechanism, as it is type sensitive, inherently type-safe, and extensible.[<sup>\[2:§38.4.1.1\]</sup>](#references)

As you will see shortly, the formatted I/O operators are simpler to use and less error-prone than the unformatted ones. So it is recommended that you should prefer formatted operators if possible.[<sup>\[2:§38.4.1.2\]</sup>](#references)

## FS: unformatted input

| std::getline(in, s)       | Read a line from an **in**put stream into a **s**tring; remove delimiter; do not modify gcount                      |
| in.get()                  | Read one character or characters from **in**put stream; do not remove delimiter                                     |
| in.getline()              | Read a line from an **in**put stream; remove delimiter; modify gcount                                               |
| in.read()                 | Read characters from **in**put stream; do not put a **0** at the end                                                |
| n = in.readsome(s, count) | Read all available, or up to **count**, characters from **in**put stream; end-of-file is not considered as an error |
| in.ignore()               | Extract and discard characters from **in**put stream; remove delimiter                                              |
| in.peek()                 | Read the next character from **in**put stream without extracting it                                                 |
| in.unget()                | Unget the most recently extracted character back into **in**put stream                                              |
| in.putback(c)             | Put the character **c** back into **in**put stream                                                                  |

## FS: unformatted output

| out.put(c)  | Write the **c**haracter to the **out**put stream |
| out.write() | Write the characters to the **out**put stream    |

## FS: buffering

By default, `fstream` is buffered. You can flush the output buffer explicitly by calling `flush()`, or by using the output manipulator `std::endl`, which inserts a newline character into the output sequence as well.

An output stream is automatically flushed when it is destroyed, when a **tie()**d `istream` needs input, and when the implementation find it advantageous.[<sup>\[2:§38.4.5.2\]</sup>](#references) For example, on most Unix system, the output buffer is automatically flushed when writing a newline character into an output stream that is a terminal.[<sup>\[3\]</sup>](#references)

To turn off buffering for a file stream completely, one can do so by using the output manipulator `std::unitbuf`. This flag is set for `std::cerr` by default. Another way to make a file stream unbuffered is calling the `out.rdbuf()->pubsetbuf(nullptr, 0)` of `filebuf` class directly, which is the public version of `setbuf()`. Just note that it has to be called before any I/O has taken place in order for it to take effect.[<sup>\[3\]</sup>](#references)


# C-style I/O

The standard library for the C language is with minor modifications incorporated into the C++ standard library. The C standard library provides quite a few functions that have proven useful over the years in a wide variety of contexts - especially for relatively low-level programming.[<sup>\[2:§43.1\]</sup>](#references) The C-style I/O functions are among them.

Thus, In C++, the other portable way for file access is by using the C++ version of the C Standard Input and Output Library, which is mainly defined in header `<cstdio>`.

## FP: open/close

| fp = std::fopen(name, mode) | Open the file by given **name** with **mode**                                                                                                              |
| std::fclose(fp)             | Close the given file stream; any unwritten buffered data are flushed to the OS; automatically called for each open file when a program terminates normally |

`std::fopen()` returns a pointer to a `std::FILE` stream object, which contains information about the file.
C standard (referenced by C++ standard) does not specify whether `std::FILE` is a complete object type. Thus, `std::FILE` may be semantically non-copyable.[<sup>\[5\]</sup>](#references)

Header `<cstdio>` contains three predefined stream objects:

| stdin  | Associated with the standard input stream  |
| stdout | Associated with the standard output stream |
| stderr | Associated with the standard error stream  |

## FP: formatted I/O

| n = std::fprintf(fp, format, ...) | Write data to output stream according to **format**                  |
| n = std::fscanf(fp, format, ...)  | Read data from input stream and interpret it according to **format** |

Using `std::fprintf()` is unsafe in the sense that type checking is not done.[<sup>\[2:§43.3\]</sup>](#references) Such as:

```cpp
{% include src/2020-05-16-a-concise-cpp-file-access-reference/fprintf-issue.hpp %}
```

Also note, the arguments to `std::scanf()` must be pointers. For example, `scanf("%d", n)` is usually not what you want, rather it should be `scanf("%d", &n)` instead.[<sup>\[6:§7.4\]</sup>](#references)

## FP: unformatted input

| c = std::getc(fp)                              | Read the next **c**haracter from the given input stream                                                                                       |
| s = std::fgets(s, n, fp)                       | Read a line from the given input stream into the character array pointed to by **s**; do not remove newline character; put a **0** at the end |
| c = std::ungetc(c, fp)                         | Put the **c**haracter back into the input buffer; Only one character of pushback is guaranteed per file                                       |
| n = std::fread(buffer, object_size, count, fp) | Read up to **count** objects into the array **buffer** from the given input stream                                                            |

Similar to `std::fputc()` and `std::putc()`, in C++, calling `std::fgetc()` and `std::getc()` always have the same effect.

Never use `std::fgets()` or its rough equivalent `std::scanf("%s", s)`! For years, they were the favorites of virus writers: By providing an input that overflows the input buffer, a program can be corrupted and a computer potentially taken over by an attacker.[<sup>\[2:§43.3\]</sup>](#references)

## FP: unformatted output

| c = std::putc(c, fp)                            | Write a **c**haracter to the given output stream                                   |
| std::fputs(s, fp)                               | Write the null-terminated **s**tring to the output stream                          |
| n = std::fwrite(buffer, object_size, count, fp) | Write up to **count** objects from the array **buffer** to the given output stream |

In C, `putc()` may be implemented as a macro, which is disallowed in C++, but `fputc()` is guaranteed to be a function. Therefore calls to `std::fputc()` and `std::putc()` always have the same effect in C++.[<sup>\[5\]</sup>](#references)

## FP: buffering

| std::fflush(fp) | Flush any unwritten data from the stream's buffer to the associated output device; If given `nullptr`, all open output streams are flushed |

C file output stream is also buffered by default. An output stream is automatically flushed when it is closed, and when a program terminates normally.

Calling `std::setbuf(fp, nullptr)`, which passes a `nullptr` as the `buffer` argument, turns off buffering. By default `stderr` stream is not fully buffered. Additionally, although not mandated by POSIX, the UNIX convention is that `stdin` and `stdout` are line-buffered if associated with a terminal and `stderr` is unbuffered.[<sup>\[5\]</sup>](#references)

## FP: C11

The C11 version of the C standard library offers a whole alternate set of stdio input functions that take an extra argument to defend against overflow, such as, gets_s(s, n), fscanf_s() and so on.[<sup>\[2:§43.3\]</sup>](#references) Since, those functions are not supported in C++ yet, I will just skip them.


# POSIX file APIs

Another feasible way of accessing files in C++ is using the low level system APIs directly. POSIX is the most standard way of doing that on Unix and Unix-like systems.

Most POSIX file I/O operations are defined in header `<unistd.h>`, except `open()` and `creat()` which are defined in `<fcntl.h>`.

## FD: open/close

| fd = open(name, flags, perms=0) | Open the given file by **name** with **flags**                                                                                                                                  |
| fd = creat(name, perms)         | Create a new file with the given **perm**ission bits or re-write an old one; equivalent to `open(name, O_CREAT|O_WRONLY|O_TRUNC, perms)`                                        |
| close(fd)                       | Close a file descriptor, so that it no longer refers to any file and may be reused; termination of a program via `exit()` or return from the `main()` closes all open files |

The return value of `open()` is a file descriptor, which is a reference to an open file description. The open file description records the file offset and the file status flags.[<sup>\[7\]</sup>](#references)

The following preprocessor symbols are defined in `<unistd.h>`:

| STDIN_FILENO  | The default file descriptor number for the standard input stream, typically 0  |
| STDOUT_FILENO | The default file descriptor number for the standard output stream, typically 1 |
| STDERR_FILENO | The default file descriptor number for the standard error stream, typically 2  |

## FD: formatted I/O

This one is simple, as there is no formatted I/O operations among the low level POSIX I/O functions.

## FD: unformatted input

| n = read(fd, buffer, count) | Read up to **count** bytes from given file into the **buffer** |

## FD: unformatted output

| n = write(fd, buffer, count) | Write up to **count** bytes from the **buffer** to file |

## FD: buffering

Unlike the C standard library and C++ standard library, there is no buffer at the application level when using the POSIX APIs to access files directly. You have to create and maintain your own buffers if needed. However, there usually are buffer cache at the kernel level.

Also note, a successful `close()` does not guarantee that the data has been successfully saved to disk, as the kernel uses the buffer cache to defer writes. Typically, filesystems do not flush buffers when a file is closed. If you need to be sure that the data is physically stored on the underlying disk, use `fsync()`.  (It will then depend on the disk hardware at this point.)[<sup>\[7\]</sup>](#references)


# Conclusion

In today's post, we have talked about how to perform file I/O operations in C++ by using the file stream classes, the C-style I/O functions and the POSIX file APIs. Each method has it own quirks and advantages, so sometimes it could be difficult to choose the right one to use. And one of the purposes of this concise reference is to try to make this process easier.


# References

1. [Input/output library](https://en.cppreference.com/w/cpp/io)
1. [The C++ Programming Language, 4th Edition (#ad)](https://www.amazon.com) by Bjarne Stroustrup
1. [The GNU C++ Library: Chapter 13. Input and Output: Stream Buffers](https://gcc.gnu.org/onlinedocs/libstdc++/manual/streambufs.html)
1. [The C++ Standard Library, Second Edition (#ad)](https://www.amazon.com) by Nicolai Josuttis
1. [C-style file input/output](https://en.cppreference.com/w/cpp/io/c)
1. [The C Programming Language, Second Edition (#ad)](https://www.amazon.com) by Brian Kernighan and Dennis Ritchie
1. [Linux manual pages: Section 2](https://man7.org/linux/man-pages/dir_all_by_section.html#man2)
