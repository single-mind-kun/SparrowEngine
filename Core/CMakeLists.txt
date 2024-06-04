cmake_minimum_required(VERSION 3.26)

add_library(sparrow_core
    Private/SpApplication.cpp
)

target_link_libraries(sparrow_core PUBLIC sparrow_platform)