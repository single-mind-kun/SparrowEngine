#ifndef SP_VK_SWAPCHAIN_H
#define SP_VK_SWAPCHAIN_H

#include "SpVKCommon.h"

namespace spe{
    class SpVKGraphicContext;
    class SpVKDevice;

    struct SurfaceInfo{
        VkSurfaceCapabilitiesKHR capabilities;
        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
    };

    class SpVKSwapchain{
    public:
        SpVKSwapchain(SpVKGraphicContext* context, SpVKDevice* device);
        ~SpVKSwapchain();

        bool ReCreate();

    private:

        void QuerySurfaceCapabilities();

        VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
        SpVKGraphicContext* mContext;
        SpVKDevice* mDevice;
        SurfaceInfo mSurfaceInfo;

        std::vector<VkImage> mImages;
    };
};


#endif