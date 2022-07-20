# config_compiler_and_linker
macro (CONFIG_CXX_COMPILER_AND_LINKER std)
    set(CMAKE_CXX_STANDARD ${std})
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if (result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif ()
endmacro ()

# cmake-lint: disable=C0307

# add_single_executable
function (add_single_executable main_name)
    add_executable(${PROJECT_NAME}_${main_name} ${main_name}.cpp ${ARGN})
    add_executable(${PROJECT_NAME}::${main_name} ALIAS ${PROJECT_NAME}_${main_name})
    target_compile_options(${PROJECT_NAME}_${main_name}
                           PRIVATE ${COMPILER_WARNING_OPTIONS})
    set_target_properties(${PROJECT_NAME}_${main_name} PROPERTIES OUTPUT_NAME
                                                                  ${main_name})
endfunction ()

# add_runnable_test
function (add_runnable_test main_name)
    if (WANT_TESTS)
        add_test(NAME ${PROJECT_NAME}.${main_name}.runnable_test
                 COMMAND ${PROJECT_NAME}::${main_name})
    endif ()
endfunction ()
