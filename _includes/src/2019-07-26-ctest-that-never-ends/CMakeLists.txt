project(
    ctest-never-ends
    VERSION 0.0.1
    LANGUAGES NONE)

find_package(Python3 COMPONENTS Interpreter)

if (Python3_FOUND)
    # set(SHOW_ISSUE true)

    if (${SHOW_ISSUE})
        add_test(NAME ${PROJECT_NAME}.real-db.setup
                 COMMAND Python3::Interpreter
                         ${CMAKE_CURRENT_SOURCE_DIR}/buggy-setup-db.py)
    else ()
        add_test(NAME ${PROJECT_NAME}.real-db.setup
                 COMMAND Python3::Interpreter
                         ${CMAKE_CURRENT_SOURCE_DIR}/setup-test-db.py)
    endif ()
    set_tests_properties(${PROJECT_NAME}.real-db.setup PROPERTIES FIXTURES_SETUP
                                                                  RealDbTests)

    add_test(NAME ${PROJECT_NAME}.real-db.cleanup
             COMMAND Python3::Interpreter
                     ${CMAKE_CURRENT_SOURCE_DIR}/cleanup-test-db.py)
    set_tests_properties(${PROJECT_NAME}.real-db.cleanup PROPERTIES FIXTURES_CLEANUP
                                                                    RealDbTests)
endif ()
