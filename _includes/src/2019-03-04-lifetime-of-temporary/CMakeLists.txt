project(
    lifetime-of-temporary
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

# add_executable_helper
function (add_executable_helper name)
    add_executable(${PROJECT_NAME}_${name} ${name}.cpp)
    add_executable(${PROJECT_NAME}::${name} ALIAS ${PROJECT_NAME}_${name})
    set_target_properties(${PROJECT_NAME}_${name} PROPERTIES OUTPUT_NAME ${name})
endfunction ()

add_executable_helper(a_temporary)
add_executable_helper(const_lvalue_reference)
add_executable_helper(rvalue_reference)
add_executable_helper(derived)
add_executable_helper(exception_1)
add_executable_helper(exception_2)
add_executable_helper(exception_3)
add_executable_helper(exception_4)
add_executable_helper(special_case)
