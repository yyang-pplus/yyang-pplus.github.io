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

add_executable(${PROJECT_NAME}_set_global_debug_level defines.hpp ldap_utils.hpp
                                                      set_global_debug_level.cpp)
add_executable(${PROJECT_NAME}::set_global_debug_level ALIAS
               ${PROJECT_NAME}_set_global_debug_level)
target_link_libraries(${PROJECT_NAME}_set_global_debug_level
                      PRIVATE ${OPENLDAP_LIBRARIES})
target_compile_options(${PROJECT_NAME}_set_global_debug_level
                       PRIVATE ${COMPILER_WARNING_OPTIONS})
set_target_properties(${PROJECT_NAME}_set_global_debug_level
                      PROPERTIES OUTPUT_NAME set_global_debug_level)

add_executable(${PROJECT_NAME}_set_handle_debug_level defines.hpp ldap_utils.hpp
                                                      set_handle_debug_level.cpp)
add_executable(${PROJECT_NAME}::set_handle_debug_level ALIAS
               ${PROJECT_NAME}_set_handle_debug_level)
target_link_libraries(${PROJECT_NAME}_set_handle_debug_level
                      PRIVATE ${OPENLDAP_LIBRARIES})
target_compile_options(${PROJECT_NAME}_set_handle_debug_level
                       PRIVATE ${COMPILER_WARNING_OPTIONS})
set_target_properties(${PROJECT_NAME}_set_handle_debug_level
                      PROPERTIES OUTPUT_NAME set_handle_debug_level)
