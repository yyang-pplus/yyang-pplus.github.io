project(
    possible-ways-to-end-program
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(catch-all-main)
add_runnable_test(catch-all-main)

add_single_executable(catch-current-exception-on-abort
                      catch-current-exception-on-terminate.hpp)
add_runnable_test(catch-current-exception-on-abort)
set_tests_properties(${PROJECT_NAME}.catch-current-exception-on-abort.runnable-test
                     PROPERTIES WILL_FAIL ON)

add_single_executable(catch-current-exception-on-terminate
                      catch-current-exception-on-terminate.hpp)
add_runnable_test(catch-current-exception-on-terminate)
set_tests_properties(${PROJECT_NAME}.catch-current-exception-on-terminate.runnable-test
                     PROPERTIES WILL_FAIL ON)
