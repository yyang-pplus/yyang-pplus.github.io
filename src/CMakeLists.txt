file(
    GLOB subprojects
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach (a_project ${subprojects})
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${a_project})
        add_subdirectory(${a_project})
    endif ()
endforeach ()
