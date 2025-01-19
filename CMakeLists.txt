include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.15)

project(cchess)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(PROJECT_LIB_NAME "lib${PROJECT_NAME}")

set(CMAKE_VERBOSE_MAKEFILE ON)

if(RUN_TESTS EQUAL 1)
    message(STATUS "RUN_TESTS enabled, building and running tests")
    enable_testing()
endif()

find_package(OpenMP)

set(libromano_DIR "${CMAKE_SOURCE_DIR}/libromano/install/cmake")
find_package(libromano)

add_subdirectory(src)

if(RUN_TESTS EQUAL 1)
    add_subdirectory(tests)
endif()