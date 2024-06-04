#ifndef SPGLFW_WINDOW_H
#define SPGLFW_WINDOW_H

#include "SpWindow.h"

class GLFWwindow;

namespace spe{
    class SpGLFWwindow : public SpWindow{
    public:
        SpGLFWwindow() = delete;
        SpGLFWwindow(uint32_t width ,uint32_t height, const char* title);
        ~SpGLFWwindow() override;
        bool ShouldClose() override;
        void PollEvents() override;
        void SwapBuffer() override;

    private:
        GLFWwindow* mGLFWwindow;

    };
}

#endif