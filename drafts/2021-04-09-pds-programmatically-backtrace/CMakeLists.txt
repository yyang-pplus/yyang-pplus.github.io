project(
    pds-programmatically-backtrace
    VERSION 0.0.1
    LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)
config_cxx_compiler_and_linker(17)
set(CMAKE_ENABLE_EXPORTS ON)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(backtrace-and-symbols backtrace-and-symbols.hpp
                      dummy-functions.hpp)
add_runnable_test(backtrace-and-symbols)

find_package(
    Boost 1.74
    COMPONENTS stacktrace_backtrace
    REQUIRED)

add_single_executable(boost-stacktrace-on-terminate dummy-functions.hpp)
target_link_libraries(${PROJECT_NAME}_boost-stacktrace-on-terminate
                      PRIVATE Boost::stacktrace_backtrace)
add_runnable_test(boost-stacktrace-on-terminate)
set_tests_properties(${PROJECT_NAME}.boost-stacktrace-on-terminate.runnable-test
                     PROPERTIES WILL_FAIL ON)

add_single_executable(boost-stacktrace-on-assert boost-stacktrace-on-assert.hpp
                      dummy-functions.hpp)
target_link_libraries(${PROJECT_NAME}_boost-stacktrace-on-assert PRIVATE backtrace)
add_runnable_test(boost-stacktrace-on-assert)
set_tests_properties(${PROJECT_NAME}.boost-stacktrace-on-assert.runnable-test
                     PROPERTIES WILL_FAIL ON)
