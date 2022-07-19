---
title: "User-Defined Input Stream Buffers"
date: "2020-06-15"
categories:
  - "cpp"
tags:
  - "buffer"
  - "c++"
  - "I/O"
  - "underflow"
  - "istream"
  - "POSIX"
  - "programming"
  - "putback"
  - "get area"
  - "software"
  - "streambuf"
---

Continue with [my preview post]({{page.previous.url}}), in today's article, I am going to explore more about user-defined input stream buffers. For reasons you will see shortly, the input stream buffers are slightly more involved than the output ones. However, our existing knowledge with the output stream buffers will definitely help us understanding the input ones. So, if you haven't, I strongly recommend you check out [my output stream buffer post]({{page.previous.url}}) first.

This post is largely inspired by a section of the similar title in [The C++ Standard Library](#references) by Nicolai Josuttis. Great book. Highly recommended.


# Unbuffered Input Stream Buffer

As usual, let's start with an unbuffered one.

```cpp
{% include src/2020-06-15-user-defined-input-stream-buffers/hex_in_stream_nobuf.hpp %}
```

Basically, **HexInBuf** class is the opposite of the **HexOutBuf** class. It implements an input stream buffer which can be used to read in hex encoded string and convert it into a normal string. As you can see, comparing to the unbuffered version of **HexOutBuf**, the unbuffered input stream buffer has to override more virtual functions.

An **istream** can **peek()** one character from its associated input stream buffer by calling its member function **sgetc()**. If no character is available in the get area, **sgetc()** returns **underflow()**. The virtual function **underflow()** is responsible for reading more data in from the underlying input channel. It returns the value of the first character read on success or **traits_type::eof()** on failure. The base class version of this function does nothing, and returns **traits_type::eof()**.[<sup>\[2\]</sup>](#references) In our example, we use the **lseek()** trick to reposition the input file offset back to its original position in order to avoid setting up a buffer.

An **istream** can read one character from its associated input stream buffer by using its member function **sbumpc()**. If no character is available in the get area, **sbumpc()** returns **uflow()**. The virtual function **uflow()** behaves similarly to **underflow()**, except it also increments the read pointer. The base class version of the function calls **underflow()** and increments the read pointer.[<sup>\[2\]</sup>](#references)

The function **snextc()** can also be used to read one character. This function first calls **sbumpc()** to advance the read pointer, then calls **sgetc()** in order to read the character.

The function **sgetn()** can be used for reading multiple characters at once. This function simply calls the virtual function **xsgetn(s, count)** of the most derived class. The default implementation of **xsgetn()** reads characters as if by repeated calls to **sbumpc()**.[<sup>\[2\]</sup>](#references) Like the function **xsputn()** for output stream buffer, overriding **xsgetn()** is only necessary if reading multiple characters can be implemented more efficiently than reading characters one at a time.

As with input stream buffer, characters can also be put back into the read buffer by using the functions **sputbackc(c)** and **sungetc()**. Both functions decrement the read pointer, if possible. The difference is that **sputbackc()** gets the character to be put back as its argument. If putback position was not available, they return what virtual function **pbackfail()** returns. By overriding this function, you can implement a mechanism to restore the old read position even in this case. The default base class version of this function does nothing and returns **traits_type::eof()** in all situations.[<sup>\[1\]</sup>](#references)[<sup>\[2\]</sup>](#references) Our version of **pbackfail()** also ensures that the given character was indeed the character read.

Here is an example of using this stream buffer:

```cpp
{% include src/2020-06-15-user-defined-input-stream-buffers/test_utils.hpp %}
```

```cpp
{% include src/2020-06-15-user-defined-input-stream-buffers/hex_in_stream_nobuf.cpp %}
```

You may run it like this:

```bash
$ ./hex_in_stream_nobuf <<< '303a09455e69'
0(48)0(48):(58) (9)E(69)E(69)^(94)i(105)
```


# Single Character Buffered Input Stream Buffer

Because of how an input stream buffer works, an unbuffered version may not be the simplest way to implement a user-defined input stream buffer, rather the simplest way would be an input stream buffer that only maintains a single character buffer. I will show you how. However, before we dive into the implementation details, we need to understand how the get area works with the operations.

The get area is defined by three pointers that can be accessed by the following three member functions:[<sup>\[1\]</sup>](#references)[<sup>\[2\]</sup>](#references)

1. **eback()**: ("**e**nd put**back**") points at the beginning of the get area, or, as the name suggests, the end of the putback area.
1. **gptr()**: ("**g**et **p**oin**t**e**r**") points to the current character in the get area.
1. **egptr()**: ("**e**nd **g**et **p**oin**t**e**r**") points to one past the end of the get area.

Function **setg(eback, gptr, egptr)** sets the values of those three pointers. Characters in range [**eback()**, **gptr()**) are those can be put back. Characters in range [**gptr()**, **egptr()**) have been transported from the underlying input device, but are still waiting for processing.[<sup>\[1\]</sup>](#references)

**gbump(offset)** can be used to reposition **gptr()** by **offset** characters relative to its current position. Although, not clearly stated in the documentation, a negative **offset** may be given to decrement the read pointer **gptr()**.

As already mentioned, **sgetc()** returns the value of **\*gptr()** if **gptr() < egptr()**, returns **underflow()**, otherwise. **sbumpc()** returns **uflow()** if **gptr() == egptr()**, or, it returns the value of **\*gptr()** and advances **gptr()**.

With all this information, now we can implement our single character buffered input stream buffer.

```cpp
{% include src/2020-06-15-user-defined-input-stream-buffers/hex_in_stream_single_buf.hpp %}
```

Well, that's basically it, we only need to override **underflow()**, the default implementation of **uflow()** just works for this one character buffer.

Also note, the putback functionality of this **HexInBuf** is not completed, as it may fail at certain situations. Can you spot the issue?


# Buffered Input Stream Buffer

Of course, a single character buffer may be easy to implement, but it is not quite efficient. Here is how we can extend the same pattern and implement a fully buffered user-defined input stream buffer.

```cpp
{% include src/2020-06-15-user-defined-input-stream-buffers/hex_in_stream_buffer.hpp %}
```

For this version of **HexInBuf**, one extra thing we need to take care of is saving the old data for putback when refreshing the get area with new characters. Often, we need to move the last few characters of the current buffer to the beginning of the buffer and appends the newly read characters thereafter.[<sup>\[1\]</sup>](#references)

Although, not strictly required, we also override virtual function **sync()**. For input streams, its behavior is implementation defined. Typically, one implementation may empty the get area and move the current file position back by the corresponding number of bytes.[<sup>\[2\]</sup>](#references)


# Conclusion

As we have already shown in today's post, implementing user-defined input stream buffers usually requires us to override more virtual functions comparing to output stream buffers. Still, the key to implement an input stream buffer is in knowing when and how to override the corresponding virtual functions to manage the get area appropriately. [The full code of this article can be found on my Github]({{site.github.repository_url}}/tree/master/_includes/src/2020-06-15-user-defined-input-stream-buffers).


# References

1. [The C++ Standard Library (#ad)](https://www.amazon.com) by Nicolai Josuttis
1. [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf)
