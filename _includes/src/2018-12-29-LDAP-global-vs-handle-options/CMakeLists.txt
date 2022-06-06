cmake_minimum_required(VERSION 3.16)

project(
    LDAP-global-vs-handle
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(17)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

set(OPENLDAP_LIBRARIES ldap lber)

# add_executable_helper
function (add_executable_helper name)
    add_executable(${PROJECT_NAME}_${name} ${name}.cpp defines.hpp ldap_utils.hpp)
    add_executable(${PROJECT_NAME}::${name} ALIAS ${PROJECT_NAME}_${name})
    target_link_libraries(${PROJECT_NAME}_${name} PRIVATE ${OPENLDAP_LIBRARIES})
    target_compile_options(${PROJECT_NAME}_${name} PRIVATE ${COMPILER_WARNING_OPTIONS})
    set_target_properties(${PROJECT_NAME}_${name} PROPERTIES OUTPUT_NAME ${name})
endfunction ()

add_executable_helper(set_global_debug_level)
add_executable_helper(set_handle_debug_level)
