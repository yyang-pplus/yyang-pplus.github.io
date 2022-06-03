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
