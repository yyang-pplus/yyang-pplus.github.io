project(
    pds-compiler-warnings
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

add_single_executable(
    pds-compiler-warnings-main
    conditional-unused-but-set-parameter-fixed.hpp
    implicit-fallthrough-fixed.hpp
    switch.hpp
    unused-function-fixed.hpp
    unused-parameter-fixed.hpp
    unused-parameter.hpp
    unused-result.hpp
    unused-variable-fixed.hpp
    unused-variable.hpp)
add_runnable_test(pds-compiler-warnings-main)
