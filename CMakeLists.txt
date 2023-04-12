cmake_minimum_required(VERSION 3.16)

project(
    yyang-pplus
    VERSION 0.0.1
    LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(utils)

# ######################################################################################
# CMake Standard Options
option(BUILD_SHARED_LIBS "Build libraries as .dll's/.so's." ON)
mark_as_advanced(CLEAR BUILD_SHARED_LIBS)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo)

# ######################################################################################
# Project Options
option(WARNINGS_AS_ERRORS "Treat all warnings as build errors." ON)

option(WANT_TESTS "Build all of the project's own tests." ON)
option(WANT_AUTO_TESTS "Automatically run tests that have been changed." ON)

if (WANT_TESTS)
    enable_testing()
    include(CTest)
    mark_as_advanced(BUILD_TESTING)

    include(GoogleTest)
endif ()

# ######################################################################################
# Dependencies
find_package(Threads REQUIRED)

add_subdirectory(3rdParty)

# ######################################################################################
# Sub-Projects
add_subdirectory(_includes)

if (WANT_TESTS)
    add_subdirectory(scripts)
endif ()
