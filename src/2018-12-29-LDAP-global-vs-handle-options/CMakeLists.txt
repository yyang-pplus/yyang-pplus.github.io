cmake_minimum_required(VERSION 3.16)

project(
    LDAP-global-vs-handle
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker()

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)
if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

set(OPENLDAP_LIBRARIES ldap lber)

add_executable(test_global_option test_global_option.cpp)
target_link_libraries(test_global_option PRIVATE ${OPENLDAP_LIBRARIES})
target_compile_options(test_global_option PUBLIC ${COMPILER_WARNING_OPTIONS})

add_executable(test_handle_option test_handle_option.cpp)
target_link_libraries(test_handle_option PRIVATE ${OPENLDAP_LIBRARIES})
target_compile_options(test_handle_option PUBLIC ${COMPILER_WARNING_OPTIONS})
