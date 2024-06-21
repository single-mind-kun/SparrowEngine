#ifndef SP_VK_GRAPHIC_H
#define SP_VK_GRAPHIC_H

#include "SpGraphicContext.h"
#include "SpVKCommon.h"

namespace spe{
    struct QueueFamilyInfo{
        int32_t queueFamilyIndex = -1;
        uint32_t queueCount;
    };

    class SpVKGraphicContext : public SpGraphicContext{
    public:
        SpVKGraphicContext(SpWindow* window);
        ~SpVKGraphicContext() override;
    private:
        void PrintPhyDeviceInfo(VkPhysicalDeviceProperties& props);
        uint32_t GetPhyDeviceScore(VkPhysicalDeviceProperties &props);


        void CreateInstance();
        void CreateSurface(SpWindow* window);
        void SelectPhyDevice();

        bool bShouldValidate = true;
        VkInstance mInstance;
        VkSurfaceKHR mSurface;

        VkPhysicalDevice mPhysicalDevice;
        VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
        QueueFamilyInfo mGraphicQueueFamily;
        QueueFamilyInfo mPresentQueueFamily;

    };
}

#endif