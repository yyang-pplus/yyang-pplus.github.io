project(
    lifetime-of-temporary
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

add_single_executable(a-temporary base.hpp)
add_runnable_test(a-temporary)

add_single_executable(const-lvalue-reference base.hpp)
add_runnable_test(const-lvalue-reference)

add_single_executable(rvalue-reference base.hpp)
add_runnable_test(rvalue-reference)

add_single_executable(derived derived.hpp)
add_runnable_test(derived)

add_single_executable(exception-1 base.hpp)
add_runnable_test(exception-1)

add_single_executable(exception-2 derived.hpp)
add_runnable_test(exception-2)

add_single_executable(exception-3 base.hpp)
add_runnable_test(exception-3)

add_single_executable(exception-4 base-wrapper.hpp)
add_runnable_test(exception-4)

add_single_executable(special-case base-wrapper.hpp)
add_runnable_test(special-case)
