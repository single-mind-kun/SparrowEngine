cmake_minimum_required(VERSION 3.26)

add_executable(SandBox Main.cpp)

target_link_libraries(SandBox PRIVATE sparrow_core)