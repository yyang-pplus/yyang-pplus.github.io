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

# enable_auto_test_command
function (enable_auto_test_command target_name tests_regex)
    if (WANT_AUTO_TESTS)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ctest --output-on-failure -R ${tests_regex}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Testing '${target_name}'"
            VERBATIM)
    endif ()
endfunction ()

# add_gtest_for
function (add_gtest_for source_name)
    if (WANT_TESTS)
        set(test_source_name "${source_name}.test")
        set(target_name "${PROJECT_NAME}.${test_source_name}")
        add_executable(${target_name} ${test_source_name}.cpp)
        target_link_libraries(${target_name} PRIVATE gtest_main ${ARGN})
        target_compile_options(${target_name} PRIVATE ${COMPILER_WARNING_OPTIONS})

        add_test(NAME ${target_name} COMMAND ${target_name})

        enable_auto_test_command(${target_name} ^${target_name}$)
    endif ()
endfunction ()

# discover_gtest_for
function (discover_gtest_for source_name)
    if (WANT_TESTS)
        set(test_source_name "${source_name}.test")
        set(target_prefix "${PROJECT_NAME}.${source_name}.")
        set(target_name "${target_prefix}test")
        add_executable(${target_name} ${test_source_name}.cpp)
        target_link_libraries(${target_name} PRIVATE gtest_main ${ARGN})
        target_compile_options(${target_name} PRIVATE ${COMPILER_WARNING_OPTIONS})

        gtest_discover_tests(
            ${target_name}
            TEST_PREFIX ${target_prefix}
            PROPERTIES LABELS ${PROJECT_NAME})

        enable_auto_test_command(${target_name} ^${target_prefix})
    endif ()
endfunction ()

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
        set(target_name "${PROJECT_NAME}_${main_name}")
        set(test_name "${PROJECT_NAME}.${main_name}.runnable-test")

        add_test(NAME ${test_name} COMMAND ${target_name})

        enable_auto_test_command(${target_name} ^${test_name}$)
    endif ()
endfunction ()
