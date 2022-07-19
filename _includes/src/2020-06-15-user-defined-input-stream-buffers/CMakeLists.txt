project(
    user-defined-istream-buffer
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(hex_in_stream_buffer hex_in_stream_buffer.hpp)
add_single_executable(hex_in_stream_nobuf hex_in_stream_nobuf.hpp)
add_single_executable(hex_in_stream_single_buf hex_in_stream_single_buf.hpp
                      test_utils.hpp)
