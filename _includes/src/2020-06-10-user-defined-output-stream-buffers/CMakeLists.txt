project(
    user-defined-ostream-buffer
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(hex_out_stream hex_out_stream.hpp str_utils.hpp test_utils.hpp)
add_single_executable(hex_out_stream_buffer hex_out_stream_buffer.hpp str_utils.hpp
                      test_utils.hpp)
add_single_executable(hex_out_stream_nobuf hex_out_stream_nobuf.hpp str_utils.hpp
                      test_utils.hpp)
add_single_executable(hex_out_stream_nobuf_improved_fd
                      hex_out_stream_nobuf_improved.hpp str_utils.hpp test_utils.hpp)
add_single_executable(hex_out_stream_nobuf_improved_path
                      hex_out_stream_nobuf_improved.hpp str_utils.hpp test_utils.hpp)
