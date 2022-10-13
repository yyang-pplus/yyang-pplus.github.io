project(
    pds-compiler-warnings
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

add_single_executable(pds_compiler_warnings)
add_runnable_test(pds_compiler_warnings)
