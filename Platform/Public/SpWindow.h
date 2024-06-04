#ifndef SPWINDOW_H
#define SPWINDOW_H

#include "SpEngine.h"

namespace spe{
    class SpWindow{
    public:
        SpWindow(const SpWindow&) = delete;
        SpWindow& operator=(const SpWindow&) = delete;

        virtual ~SpWindow() = default;
        static std::unique_ptr<SpWindow> Create(uint32_t width, uint32_t height, const char* title);

        virtual bool ShouldClose() = 0;
        virtual void PollEvents() = 0;
        virtual void SwapBuffer() = 0;

    protected:
        SpWindow() = default;

    };
}


#endif