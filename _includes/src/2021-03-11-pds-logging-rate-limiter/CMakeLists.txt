project(
    pds-logging-rate-limiter
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

# add_executable_helper
function (add_executable_helper main_name)
    add_single_executable(${main_name} ${ARGN})
    add_runnable_test(${main_name})
    set_property(TEST ${PROJECT_NAME}.${main_name}.runnable-test
                 PROPERTY ENVIRONMENT "LIMITED=True")
endfunction ()

add_executable_helper(leaky-bucket-main chrono-utils.hpp logpp.hpp rate.hpp
                      leaky-bucket-logger.hpp test-utils.hpp)

add_executable_helper(sliding-log-main chrono-utils.hpp logpp.hpp rate.hpp
                      sliding-log-logger.hpp test-utils.hpp)

add_executable_helper(sliding-window-counter-main chrono-utils.hpp logpp.hpp rate.hpp
                      sliding-window-counter-logger.hpp test-utils.hpp)

add_executable_helper(
    token-bucket-main
    chrono-utils.hpp
    logpp.hpp
    rate.hpp
    token-bucket-logger.hpp
    token-bucket.hpp
    test-utils.hpp
    token.hpp)
