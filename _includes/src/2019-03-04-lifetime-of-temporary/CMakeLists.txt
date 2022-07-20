project(
    lifetime-of-temporary
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

add_single_executable(a_temporary)
add_single_executable(const_lvalue_reference)
add_single_executable(rvalue_reference)
add_single_executable(derived)
add_single_executable(exception_1)
add_single_executable(exception_2)
add_single_executable(exception_3)
add_single_executable(exception_4)
add_single_executable(special_case)

add_runnable_test(a_temporary)
add_runnable_test(const_lvalue_reference)
add_runnable_test(rvalue_reference)
add_runnable_test(derived)
add_runnable_test(exception_1)
add_runnable_test(exception_2)
add_runnable_test(exception_3)
add_runnable_test(exception_4)
add_runnable_test(special_case)
