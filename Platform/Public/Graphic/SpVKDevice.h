#ifndef SP_VK_DEVICE_H
#define SP_VK_DEVICE_H

#include "SpVKCommon.h"

namespace spe{
    class SpVKGraphicContext;
    class SpVKQueue;

    struct SpVkDeviceSettings{
        VkFormat surfaceFormat = VK_FORMAT_B8G8R8A8_UNORM;
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        uint32_t swapchainImageCount = 3;
    };

    class SpVKDevice{
    public:
        SpVKDevice(SpVKGraphicContext* context, uint32_t graphicQueueCount, uint32_t presentQueueCount, const SpVkDeviceSettings& settings = {});
        ~SpVKDevice();

        VkDevice GetDevice() const {return mDevice;}
        const SpVkDeviceSettings& GetSettings() const {return mSettings;}

    private:
        VkDevice mDevice = VK_NULL_HANDLE;
        std::vector<std::shared_ptr<SpVKQueue>> mGraphicQueues;
        std::vector<std::shared_ptr<SpVKQueue>> mPresentQueues;

        SpVkDeviceSettings mSettings;
    };
}

#endif