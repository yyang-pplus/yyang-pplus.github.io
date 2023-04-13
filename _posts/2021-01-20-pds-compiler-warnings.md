---
title: "PDS: Compiler Warnings"
date: "2021-01-20"
categories:
  - "cpp"
tags:
  - "c"
  - "c++"
  - "clang"
  - "cmake"
  - "engineering"
  - "g++"
  - "Linux"
  - "practical-debugging-skills"
  - "programming"
  - "software"
---

Compiler warnings are messages produced by a compiler who is trying to warn you about potential issues in your code. Although, warnings don't interrupt the compilation process, they should not just be ignored, as warnings may provide valuable insight and advice to keep your programs away from certain bugs. Paying attention to compiler warnings and keeping a low number of warnings is a commonly used preemptive technique to prevent bugs in the earlier phase of software development life circle. As it is said, it is easier and cheaper to fix bugs in earlier stages rather than later ones.


# Enable Warnings

Compiler warnings are important, but most useful warnings are not enabled by default. To turn on more warnings, you need to set compiler-specific flags. Here is a concise reference of some common warning flags:[<sup>\[1\]</sup>](#references)

* `-Wall`: As it is said, a better name for this flag would be `-Wcommon`,[<sup>\[2\]</sup>](#references) as this flag does not actually enable all the warnings. This flag enables warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.
* `-Wextra`: This enables some extra warning flags that are not enabled by `-Wall`.
* `-Wpedantic`: This issues all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++.
* `-Weverything`: This is a clang specific flag, which really enables all warnings.

For the complete list of all the warning flags, please consult your compiler's manuals.

## Warnings in Moderation

To quote Oscar Wilde:
> everything in moderation, including moderation.

This piece of advice applies to compiler warnings as well. Because, each warning diagnostic that is enabled for a project will consume certain amount of development resources to maintain, and having too many warnings enabled will soon become a burden of its own. This is especially true for `-Weverything`. Thus, using of `-Weverything` is generally not recommended. It should only be used as a way to find the names of interesting warning flags.[<sup>\[4\]</sup>](#references)

**Which warnings should be enabled?** That certainly depends on your use cases. In general, start with `-Wall` and `-Wextra` is a good choice for most projects. Enable `-Wpedantic` if you are aiming for ISO C/C++ conformance. Then, gradually add individual warning that you found useful by combining the `-W` prefix with the warning name. For a list of other useful warning options, which are not included in `-Wall` or `-Wextra`, please refer to the reference material [\[5\]](#references).


# Warnings as Errors

One way to enforce a zero warning policy for your project is turning all the warnings into errors by using the `-Werror` flag. Which means you have to clear all warnings to make your program compile. But, be aware, this approach does introduce a new issue, as using `-Werror` also means that changing or updating your compiler is probably more difficult. In general, it is advised to make `-Werror` optional by wrapping it with a build option.


# Warnings and CMake

For modern CMake, the right way to enable compiler warnings should be something like this:

```cmake
set(COMPILER_WARNING_OPTIONS -Wall -Wextra)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

target_compile_options(my_target PRIVATE ${COMPILER_WARNING_OPTIONS})
```

Here, `WARNINGS_AS_ERRORS` is a build option to turn warnings into errors. `my_target` is the name of the CMake target whose warnings you want to enable, this target must have been created by a command such as `add_executable()` or `add_library()` and must not be an ALIAS target.[<sup>\[6\]</sup>](#references) `PRIVATE` limits those flags to this target only and prevents them from affecting projects linking to your target.


# Suppress Warnings

Warnings are warnings for reasons, which is because they may produce false positive results. In such cases, you can either explicitly disable the offending diagnostic as a whole with its corresponding `-Wno-` option if it produces too much noise, or suppress each occurrence with certain techniques. However, before doing that, you should make sure those warnings are indeed not caused by incorrect code.

## Suppress unused parameter warnings

`-Wunused-parameter` or `-Wunused-but-set-parameter` could be useful to prevent bugs which are results of parameter names that are easily misread, a silly example would be:

```cpp
{% include src/2021-01-20-pds-compiler-warnings/unused-parameter.hpp %}
```

Most likely, the above function was intended to return the sum of the given two parameters, instead of the double of only the first parameter.

However, there are cases where unused parameters cannot be avoid, such as functions that have to conform to certain predefined interfaces. In that case, the recommended way to suppress this kind of warnings is to make unused parameters unnamed.[<sup>\[7 F.9\]</sup>](#references)

```cpp
{% include src/2021-01-20-pds-compiler-warnings/unused-parameter-fixed.hpp %}
```

Additionally, if parameters are conditionally unused, declare them with the `[[maybe_unused]]` attribute. For example:[<sup>\[7 F.9\]</sup>](#references)

```cpp
{% include src/2021-01-20-pds-compiler-warnings/conditional-unused-but-set-parameter-fixed.hpp %}
```

## Suppress unused variable warnings

Similar to the unused parameter warnings, `-Wunused-variable` may help you to detect issues those are related to unintentionally accessing the wrong variables. For example:

```cpp
{% include src/2021-01-20-pds-compiler-warnings/unused-variable.hpp %}
```

The first `i` is never used, as it is shadowed by the second `i`. But, sometimes, it is possible that some variables are intentionally not used in all build targets, like, the following use of `assert()`:

```cpp
{% include src/2021-01-20-pds-compiler-warnings/unused-variable-fixed.hpp %}
```

As you can see, `[[maybe_unused]]` can be used to suppress unused variable warnings as well.

## Suppress unused function warnings

Like the unused variable warnings, `-Wunused-function` could be helpful from time to time, and the `[[maybe_unused]]` attribute can also be useful here if you really want to suppress the unused function warnings for a specific set of functions.

```cpp
{% include src/2021-01-20-pds-compiler-warnings/unused-function-fixed.hpp %}
```

## Suppress unused result warnings

`-Wunused-result` is given when you ignore the return value of a function that is declared with attribute `[[nodiscard]]`. The following function as an example:

```cpp
{% include src/2021-01-20-pds-compiler-warnings/unused-result.hpp %}
```

The `[[nodiscard]]` here is trying to encourage the callers to remember to test the returned error code, as the `connect()` may fail. Usually, there is a good reason why the author of the function used `[[nodiscard]]` in the first place, so think twice before you discard such a result. If you still think it’s appropriate and your code reviewer agrees, use `std::ignore =` to turn off the warning. This way is simple, portable, and easy to grep.[<sup>\[7 ES.48\]</sup>](#references)

```cpp
#include <tuple>

std::ignore = connect();
```

## Suppress switch warnings

The `-Wswitch` warnings warn you about enumeration values that are not handled in a switch statement. This kind of warnings usually happens when you added a new enumeration value to an enumerated type, but forgot to add a corresponding case label to the switch statement. For example:

```cpp
{% include src/2021-01-20-pds-compiler-warnings/switch.hpp %}
```

Although, it is easy to suppress this warning with a `default` label, but it is not recommended, as that will also suppress any diagnostics which may detect genuine mistakes in the future. In general, it is better to just add the missing enumeration value case labels explicitly.

## Suppress implicit fallthrough warnings

`-Wimplicit-fallthrough` warnings could be useful in case you forgot to add a `break` for one case label before reaching the next one. If you are sure that is intended, you can suppress this warning with `[[fallthrough]]`.

```cpp
{% include src/2021-01-20-pds-compiler-warnings/implicit-fallthrough-fixed.hpp %}
```


# Conclusion

In today's post, I have talked about the importance of enabling the right set of compiler warnings in order to reduce the number of defects in your code base. I have also showed a few commonly used techniques to suppress certain warnings when necessary. I hope you found this post useful, if so, you may also want to check out [other articles which also belong to the PDS series](/pds). <!-- JEKYLL_RELATIVE_URL_CHECK_SKIP_LINE -->


# References

1. [Using the GNU Compiler Collection (GCC): Options to Request or Suppress Warnings](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
1. [Tutorial: Managing Compiler Warnings with CMake](https://www.foonathan.net/2018/10/cmake-warnings/) by Jonathan Müller
1. [Clang Compiler User’s Manual](https://clang.llvm.org/docs/UsersManual.html)
1. [Don’t put -Weverything in your build flags](https://quuxplusone.github.io/blog/2018/12/06/dont-use-weverything/) by Arthur O’Dwyer
1. [Useful GCC warning options not enabled by -Wall -Wextra](https://kristerw.blogspot.com/2017/09/useful-gcc-warning-options-not-enabled.html) by Krister Walfridsson
1. [CMake Documentation: cmake-commands: target_compile_options](https://cmake.org/cmake/help/latest/command/target_compile_options.html)
1. [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) by Bjarne Stroustrup and Herb Sutter
