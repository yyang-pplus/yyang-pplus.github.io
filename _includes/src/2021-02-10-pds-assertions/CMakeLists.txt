project(
    pds-assertions
    VERSION 0.0.1
    LANGUAGES C CXX)

config_cxx_compiler_and_linker(20)

set(COMPILER_WARNING_OPTIONS -Wall -Wextra -pedantic-errors)

add_single_executable(show-assertion)

if (WARNINGS_AS_ERRORS)
    list(APPEND COMPILER_WARNING_OPTIONS -Werror)
endif ()

add_single_executable(
    main assert-on-concept.hpp assertion-as-comment.hpp assertion-as-precondition.hpp
    if-as-precondition.hpp expects-as-precondition.hpp)
target_link_libraries(${PROJECT_NAME}_main PUBLIC Microsoft.GSL::GSL)
add_runnable_test(main)

add_single_executable(assert-before-crash assert-before-crash.hpp)

discover_gtest_for(assert-false)