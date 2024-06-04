#include "Window/SpGLFWwindow.h"
#include "SpLog.h"
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"
namespace spe{
    SpGLFWwindow::SpGLFWwindow(uint32_t width, uint32_t height, const char *title) {
        if(!glfwInit()){
            SpLogError("Failed to init glfw.");
            return;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        mGLFWwindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if(!mGLFWwindow){
            SpLogError("Failed to create glfw window.");
            return;
        }

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if(primaryMonitor){
            int xPos, yPos, workWidth, workHeight;
            glfwGetMonitorWorkarea(primaryMonitor, &xPos, &yPos, &workWidth, &workHeight);
            glfwSetWindowPos(mGLFWwindow, workWidth / 2 - width / 2, workHeight / 2 - height / 2);
        }

        glfwMakeContextCurrent(mGLFWwindow);

        glfwShowWindow(mGLFWwindow);
    }

    SpGLFWwindow::~SpGLFWwindow() {
        glfwDestroyWindow(mGLFWwindow);
        glfwTerminate();
    }

    bool SpGLFWwindow::ShouldClose() {
        return glfwWindowShouldClose(mGLFWwindow);
    }

    void SpGLFWwindow::PollEvents() {
        glfwPollEvents();
    }

    void SpGLFWwindow::SwapBuffer() {
        glfwSwapBuffers(mGLFWwindow);
    }
}