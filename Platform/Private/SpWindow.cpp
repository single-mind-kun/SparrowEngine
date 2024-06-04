#include "SpWindow.h"
#include "Window/SpGLFWwindow.h"

namespace spe{
    std::unique_ptr<SpWindow> SpWindow::Create(uint32_t width, uint32_t height, const char *title) {

#ifdef SE_ENGINE_PLATFORM_WIN32
        return  std::make_unique<SpGLFWwindow>(width, height, title);
#elif SE_ENGINE_PLATFORM_MACOS
        return  std::make_unique<SpGLFWwindow>(width, height, title);
#elif SE_ENGINE_PLATFORM_LINUX
        return  std::make_unique<SpGLFWwindow>(width, height, title);
#endif

        return nullptr;
    }
}