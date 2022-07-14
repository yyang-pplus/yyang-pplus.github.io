project(
    user-defined-ostream-buffer
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

# add_executable_helper
function (add_executable_helper name)
    add_executable(${PROJECT_NAME}_${name} ${name}.cpp)
    add_executable(${PROJECT_NAME}::${name} ALIAS ${PROJECT_NAME}_${name})
    target_compile_options(${PROJECT_NAME}_${name} PRIVATE ${COMPILER_WARNING_OPTIONS})
    set_target_properties(${PROJECT_NAME}_${name} PROPERTIES OUTPUT_NAME ${name})
endfunction ()

add_executable_helper(hex_out_stream)
add_executable_helper(hex_out_stream_buffer)
add_executable_helper(hex_out_stream_nobuf)
add_executable_helper(hex_out_stream_nobuf_improved_fd)
add_executable_helper(hex_out_stream_nobuf_improved_path)
