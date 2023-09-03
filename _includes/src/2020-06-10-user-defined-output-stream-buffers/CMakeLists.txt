project(
    user-defined-ostream-buffer
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(hex-out-stream hex-out-stream.hpp str-utils.hpp test-utils.hpp)
add_runnable_test(hex-out-stream)

add_single_executable(hex-out-stream-buffer hex-out-stream-buffer.hpp str-utils.hpp
                      test-utils.hpp)
add_runnable_test(hex-out-stream-buffer)

add_single_executable(hex-out-stream-nobuf hex-out-stream-nobuf.hpp str-utils.hpp
                      test-utils.hpp)
add_runnable_test(hex-out-stream-nobuf)

add_single_executable(hex-out-stream-nobuf-improved-fd
                      hex-out-stream-nobuf-improved.hpp str-utils.hpp test-utils.hpp)
add_runnable_test(hex-out-stream-nobuf-improved-fd)

add_single_executable(hex-out-stream-nobuf-improved-path
                      hex-out-stream-nobuf-improved.hpp str-utils.hpp test-utils.hpp)
add_runnable_test(hex-out-stream-nobuf-improved-path)
