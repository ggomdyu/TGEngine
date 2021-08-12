cmake_minimum_required(VERSION 3.2)

project(icu)

file(GLOB_RECURSE SRC_PATH_LIST source/common/*.c source/common/*.cpp source/common/*.h)
add_library(icu ${SRC_PATH_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_PATH_LIST})

set_target_properties(icu PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(icu PUBLIC source/common)
target_include_directories(icu PUBLIC source/i18n)