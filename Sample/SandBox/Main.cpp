#include <iostream>
#include "SpLog.h"
#include "SpWindow.h"
#include "SpGraphicContext.h"
#include "Graphic/SpVKGraphicContext.h"
#include "Graphic/SpVKDevice.h"
#include "Graphic/SpVKSwapchain.h"

int main(){
    std::cout << "Hellow Sparrow Engine." << std::endl;

    spe::SpLog::Init();
//    spe::SpLog::GetLoggerInstance()->trace("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    SpLogTrace("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    SpLogDebug("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    SpLogInfo("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    SpLogWarning("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    SpLogError("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);

    std::unique_ptr<spe::SpWindow> window = spe::SpWindow::Create(800, 600, "SandBox");
    std::unique_ptr<spe::SpGraphicContext> graphicContext = spe::SpGraphicContext::Create(window.get());
    spe::SpVKGraphicContext* vkContext = dynamic_cast<spe::SpVKGraphicContext*>(graphicContext.get());
    std::shared_ptr<spe::SpVKDevice> device = std::make_shared<spe::SpVKDevice>(vkContext, 1, 1);
    std::shared_ptr<spe::SpVKSwapchain> swapchain = std::make_shared<spe::SpVKSwapchain>(vkContext, device.get());

    while(!window->ShouldClose()){
        window->PollEvents();
        window->SwapBuffer();
    }

    return EXIT_SUCCESS;
}