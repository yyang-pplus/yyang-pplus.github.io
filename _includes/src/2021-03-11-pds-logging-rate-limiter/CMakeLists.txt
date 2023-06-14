project(
    pds-logging-rate-limiter
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(leaky-bucket-main chrono-utils.hpp logpp.hpp rate.hpp
                      leaky-bucket-logger.hpp test-utils.hpp)

add_single_executable(sliding-log-main chrono-utils.hpp logpp.hpp rate.hpp
                      sliding-log-logger.hpp test-utils.hpp)

add_single_executable(sliding-window-counter-main chrono-utils.hpp logpp.hpp rate.hpp
                      sliding-window-counter-logger.hpp test-utils.hpp)

add_single_executable(
    token-bucket-main
    chrono-utils.hpp
    logpp.hpp
    rate.hpp
    token-bucket-logger.hpp
    token-bucket.hpp
    test-utils.hpp
    token.hpp)
