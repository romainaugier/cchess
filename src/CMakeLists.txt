include(target_options)

file(GLOB_RECURSE sources *.c)
list(REMOVE_ITEM sources "main.c")

add_library(${PROJECT_LIB_NAME} SHARED ${sources})

set_target_options(${PROJECT_LIB_NAME})

if(WIN32)
    set_target_properties(${PROJECT_LIB_NAME} PROPERTIES CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_compile_definitions(${PROJECT_LIB_NAME} PRIVATE -DCCHESS_BUILD_SHARED)
target_include_directories(${PROJECT_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

if(OpenMP_C_Found)
    target_link_libraries(${PROJECT_LIB_NAME} PUBLIC OpenMP::OpenMP_C)
endif()

add_executable(${PROJECT_NAME} main.c)

set_target_options(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_NAME})