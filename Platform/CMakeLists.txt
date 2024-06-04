cmake_minimum_required(VERSION 3.26)

add_library(sparrow_platform
        Private/SpLog.cpp
)

target_include_directories(sparrow_platform PUBLIC External)