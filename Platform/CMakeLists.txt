cmake_minimum_required(VERSION 3.26)

add_library(sparrow_platform
        Private/SpLog.cpp
        Private/SpWindow.cpp
        Private/Window/SpGLFWwindow.cpp
)

target_include_directories(sparrow_platform PUBLIC External)

#glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(External/glfw)
target_link_libraries(sparrow_platform PRIVATE glfw)