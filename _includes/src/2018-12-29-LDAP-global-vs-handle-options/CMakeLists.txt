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
    add_single_executable(${name} defines.hpp ldap_utils.hpp)
    target_link_libraries(${PROJECT_NAME}_${name} PRIVATE ${OPENLDAP_LIBRARIES})

    add_runnable_test(${name})
endfunction ()

add_executable_helper(set_global_debug_level)
add_executable_helper(set_handle_debug_level)
