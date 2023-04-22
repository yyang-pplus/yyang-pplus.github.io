---
title: "User-Defined Output Stream Buffers"
date: "2020-06-10"
categories:
  - "cpp"
tags:
  - "buffer"
  - "c++"
  - "I/O"
  - "overflow"
  - "ostream"
  - "POSIX"
  - "programming"
  - "put area"
  - "software"
  - "streambuf"
---

You are probably already familiar with using `iostream` to perform I/O operations in C++. However, are you familiar with the backbone of the stream, which is the stream buffers. `streambuf` class is definitely a sophisticated class in the C++ standard library, if not the most sophisticated one. As it said, the complexity of the interfaces reflects tradition, the need for I/O performance, and the variety of human expectations.[<sup>\[2\]</sup>](#references) Nevertheless, a solid understanding of the stream buffer's operations is required for implementing your own stream buffers. In today's post, I am going to slice the `streambuf` into bite-size pieces and show you how to implement a user-defined output stream buffer.

This post is largely inspired by a section of the similar title in [The C++ Standard Library](#references) by Nicolai Josuttis. Great book. Highly recommended.


# Stream buffer basics

```cpp
template<class CharT, class Traits = std::char_traits<CharT>>
class basic_streambuf;
```

Found in `<streambuf>`, the template class `basic_streambuf<>` defines the interface for stream buffers. A **stream buffer** is an abstract layer between an I/O stream and the final data source or destination. Different `streambuf` subclass implement different buffering strategies. Typically, an output stream buffer stores characters gathered from an output stream in a buffer until it **flush**es those characters to their real destination. An input stream buffer is similar, expect that the characters flow the other way.[<sup>\[2\]</sup>](#references) The buffer used to write characters is also called **put area**, while the buffer for input is also called **get area**. The key to understand stream buffer's operations is in knowing how those functions manipulate the get area or the put area.


# Unbuffered output stream buffer

Streambuf buffer management is fairly sophisticated. So, let's start with a simple one, which has no buffer to manage.

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream-nobuf.hpp %}
```

Basically, `HexOutBuf` class is a stream buffer which converts each character to its two character hex representation before writing it to the standard output channel(`1`) using the POSIX API `write()`.

`ToHex()` is the function converts the given character to its Base16 encoding, and it may be implemented like this:

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/str-utils.hpp %}
```

You can try this `HexOutBuf` with the following example program:

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/test-utils.hpp %}
```

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream-nobuf.cpp %}
```

The output may look like this:

```bash
$ ./hex-out-stream-nobuf
3132333400494a4b006162000a00
```

As you can see, the key to implement an output stream buffer is in overriding the `overflow()` virtual function. `overflow(c)` is responsible for sending the characters currently in the buffer, if any, plus the given character `c` to their real destination. It gets called when there is no room left in the associated put area. In our case, the default constructor assigns no space to the put area, thus `overflow()` is called for each character as soon as it is received.

Also, note that `overflow()` returns unspecified value not equal to `traits_type::eof()` on success, `traits_type::eof()` on failure. The base class version of the function returns `traits_type::eof()`.[<sup>\[3\]</sup>](#references)


# Unbuffered output stream buffer improved

Although, our simple output stream buffer `HexOutBuf` works perfectly fine, it is not quite flexible. As it can only write to the standard output channel. Here is how we can improve it.

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream-nobuf-improved.hpp %}
```

The main improvement over the [previous](#unbuffered-output-stream-buffer) version is the added constructors and destructor. One of the constructor takes a file descriptor `fd`, and assume that file descriptor is owned by someone else. The other constructor takes a few arguments which are used to `open()` a new file descriptor. The destructor simply `close()` the current associated file descriptor if it is owned by the stream buffer. This version of `HexOutBuf` serves as a good example of showing that a stream buffer can either own its associated underlying I/O channel, or not.

Also, since copy and move semantics are not the main topic of this post, I will not discuss them further here. The related functionalities are provided in the above example just for completeness.

A sample program to show a stream buffer that owns the I/O channel:

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream-nobuf-improved-path.cpp %}
```

A sample application to show a stream buffer which does not own its I/O channel:

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream-nobuf-improved-fd.cpp %}
```


# Output stream

Although, not strictly required, it is convenient to also define a special stream class that mainly forwards the constructor arguments to the corresponding stream buffer. The following example demonstrates that.[<sup>\[1\]</sup>](#references)

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream.hpp %}
```

Note, in the member initializer list, a `nullptr` has to be passed to the base class at first, because at this point, the stream buffer member has not been fully initialized. Later in the body of the constructor, we can associate the fully initialized stream buffer with the stream by calling the member function `rdbuf()`.

`setstate()` is used to set the stream error flags in the event of a failure.

This output stream may be used like:

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream.cpp %}
```

That is pretty much all you need to know about the unbuffered output stream buffer, except member function `sputc()` and `sputn()`. We will talk about them in the [next section](#buffered-output-stream-buffer), where we are going to look at a stream buffer that actually buffers.

# Buffered output stream buffer

The put area is defined by three pointers that can be accessed by the following three member functions:[<sup>\[1\]</sup>](#references)

1. `pbase()`: ("**p**ut **base**") points at the beginning of the output buffer.
1. `pptr()`: ("**p**ut **p**oin**t**e**r**") points at the position that is the next candidate for writing.
1. `epptr()`: ("**e**nd **p**ut **p**oin**t**e**r**") points to one past the end of the buffer.

Those pointers can be initialized by calling the member function `setp(begin, end)`. The constructor of the base class simply sets no buffer.

`pbump(offset)` can be used to reposition `pptr()` by `offset` characters relative to its current position. The `offset` may be positive or negative.

The **p** suffix of above mentioned functions is needed because we can create an `iostream` derived from both `istream` and `ostream`, and such a stream needs to keep track of both a get position and a put position.[<sup>\[2\]</sup>](#references)

An `ostream` can send one character to its associated output stream buffer by calling its member function `sputc(c)`. If `pptr() != epptr()`, that character is copied to `*pptr()`, then `pptr()` is incremented. Otherwise, if `pptr() == epptr()`, `overflow()` is called.[<sup>\[1\]</sup>](#references)

An `ostream` can send multiple characters to the output stream buffer at once by using the member function `sputn(s, n)`, which simply calls the virtual function `xsputn(s, n)` of the most derived class. The base class version of the function calls `sputc()` for each character. Often, overriding `xsputn()` is only necessary if writing multiple characters can be implemented more efficiently than writing characters one at a time.[<sup>\[1\]</sup>](#references) For example, in some implementations, `std::ofstream::write()` simply passes the pointer to the suitable system call without intermediate buffering,[<sup>\[3\]</sup>](#references) something equivalent to this:

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/filebuf-xsputn.hpp %}
```

Also note, there is no corresponding `xsputc()` virtual function, thus it is not possible to alter `sputc()` function's behavior through derived class.

With all this information, now we can implement our buffering stream buffer.

```cpp
{% include src/2020-06-10-user-defined-output-stream-buffers/hex-out-stream-buffer.hpp %}
```

Note the `-1` when calling `setp()` in the constructor, that is because, when `overflow()` gets called, it not only flushes the current content of the buffer, but also the given character. Thus, it is pretty convenient to leave at least one space for this character, so that, it can also be stored in the buffer and the whole buffer can then be written to the output channel with just one system call.

Also note, the `write()` POSIX API used in `flushBuffer()`, returns the number of bytes written on success. It is not uncommon for `write()` to transfer fewer than the requested number of bytes, especially for socket or pipe. Normally, when a partial write happens, the caller should make another `write()` call to transfer the remaining bytes. However, here, to keep things simple, we just treat all partial writes as errors.

We override the virtual function `sync()`, as well. For output streams, this function is responsible for flushing the buffer. For the unbuffered versions of the stream buffer, overriding this function is not necessary, because there is no buffer to be flushed. `sync()` is also called by the destructor to ensure that buffer gets flushed when the stream buffer is destroyed.[<sup>\[1\]</sup>](#references) `sync()` returns 0 on success, -1 otherwise. The base class version of this function has no effect, and returns 0.[<sup>\[3\]</sup>](#references)

Although, not implemented in our `HexOutBuf`, the virtual functions `seekoff()` and `seekpos()` may be overridden to allow manipulation of the write position if such operations are also honored by the underlying I/O devices. The base class version of these functions have no effect.


# Conclusion

In today's post, I have shown you a few examples of user-defined output stream buffers, from a simple unbuffered one to a slightly more completed buffered one. The key to implement an output stream buffer is in understanding when and how to override the corresponding virtual functions to manipulate the put area, if any, appropriately. [The complete code for this article can be found on my Github]({{site.github.repository_url}}/tree/master/_includes/src/2020-06-10-user-defined-output-stream-buffers).

Unfortunately, output stream buffers are only half of the story. Guess what's the other half? Yes, the input stream buffers. For various reasons, the input stream buffers are a bit more involved than the output stream buffers. But, do not worry. I will have you covered, in [my next post]({{page.next.url}}).


# References

1. [The C++ Standard Library (#ad)](https://www.amazon.com) by Nicolai Josuttis
1. [The C++ Programming Language, 4th Edition (#ad)](https://www.amazon.com) by Bjarne Stroustrup
1. [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf)
