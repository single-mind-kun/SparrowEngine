cmake_minimum_required(VERSION 3.26)

add_library(sparrow_platform
        Private/SpLog.cpp
        Private/SpWindow.cpp
        Private/Window/SpGLFWwindow.cpp

        Private/SpGraphicContext.cpp
        Private/Graphic/SpVKGraphicContext.cpp
        Private/Graphic/SpVKDevice.cpp
        Private/Graphic/SpVKQueue.cpp
        Private/Graphic/SpVKSwapchain.cpp
)

target_include_directories(sparrow_platform PUBLIC External)

#glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(External/glfw)
target_link_libraries(sparrow_platform PRIVATE glfw)

find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    message("----> Find vulkan success: ${Vulkan_INCLUDE_DIRS}")
endif()
target_include_directories(sparrow_platform PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(sparrow_platform PRIVATE ${Vulkan_LIBRARY})
