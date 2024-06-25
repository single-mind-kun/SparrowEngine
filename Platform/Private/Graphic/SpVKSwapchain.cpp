#include "Graphic/SpVKSwapchain.h"
#include "Graphic/SpVKGraphicContext.h"
#include "Graphic/SpVKDevice.h"


namespace spe{
    SpVKSwapchain::SpVKSwapchain(SpVKGraphicContext *context, SpVKDevice *device)
    :mContext(context), mDevice(device){
        ReCreate();
    }

    SpVKSwapchain::~SpVKSwapchain() {
        vkDestroySwapchainKHR(mDevice->GetDevice(), mSwapchain, nullptr);
    }

    bool SpVKSwapchain::ReCreate() {
        SpLogDebug("-----------------------------");
        QuerySurfaceCapabilities();
        SpLogDebug("currentExtent : {0} x {1}", mSurfaceInfo.capabilities.currentExtent.width, mSurfaceInfo.capabilities.currentExtent.height);
        SpLogDebug("surfaceFormat : {0}", vk_format_string(mSurfaceInfo.surfaceFormat.format));
        SpLogDebug("presentMode   : {0}", vk_present_mode_string(mSurfaceInfo.presentMode));
        SpLogDebug("-----------------------------");

        uint32_t imageCount = mDevice->GetSettings().swapchainImageCount;
        if(imageCount < mSurfaceInfo.capabilities.minImageCount && mSurfaceInfo.capabilities.minImageCount > 0){
            imageCount = mSurfaceInfo.capabilities.minImageCount;
        }
        if(imageCount > mSurfaceInfo.capabilities.maxImageCount && mSurfaceInfo.capabilities.maxImageCount > 0){
            imageCount = mSurfaceInfo.capabilities.maxImageCount;
        }

        VkSharingMode imageSharingMode;
        uint32_t queueFamilyIndexCount;
        uint32_t queueFamilyIndices[2] = {0, 0};
        if(mContext->IsSameGraphicPresentQueueFamily()){
            imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            queueFamilyIndexCount = 0;
        } else{
            imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            queueFamilyIndexCount = 2;
            queueFamilyIndices[0] = mContext->GetGraphicQueueFamilyInfo().queueFamilyIndex;
            queueFamilyIndices[1] = mContext->GetPresentQueueFamilyInfo().queueFamilyIndex;
        }

        std::shared_ptr<VkSwapchainKHR> sOldSwapchain = std::make_shared<VkSwapchainKHR>(mSwapchain);

        VkSwapchainCreateInfoKHR swapchainInfo = {
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .pNext = nullptr,
                .flags = 0,
                .surface = mContext->GetSurface(),
                .minImageCount = imageCount,
                .imageFormat = mSurfaceInfo.surfaceFormat.format,
                .imageColorSpace = mSurfaceInfo.surfaceFormat.colorSpace,
                .imageExtent = mSurfaceInfo.capabilities.currentExtent,
                .imageArrayLayers = 1,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .imageSharingMode = imageSharingMode,
                .queueFamilyIndexCount = queueFamilyIndexCount,
                .pQueueFamilyIndices = queueFamilyIndices,
                .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode = mSurfaceInfo.presentMode,
                .clipped = VK_FALSE,
                .oldSwapchain = *sOldSwapchain.get()
        };
        VkResult ret = vkCreateSwapchainKHR(mDevice->GetDevice(), &swapchainInfo, nullptr, &mSwapchain);
        if(ret != VK_SUCCESS){
            SpLogError("{0} : {1}", __FUNCTION__, vk_result_string(ret));
            return false;
        }
        SpLogTrace("Swapchain {0} : old: {1}, new: {2}, image count: {3}, format: {4}, present mode : {5}", __FUNCTION__, (void*)sOldSwapchain.get(), (void*)mSwapchain, imageCount,
              vk_format_string(mSurfaceInfo.surfaceFormat.format), vk_present_mode_string(mSurfaceInfo.presentMode));

        uint32_t swapchainImageCount;
        ret = vkGetSwapchainImagesKHR(mDevice->GetDevice(), mSwapchain, &swapchainImageCount, nullptr);
        mImages.resize(swapchainImageCount);
        ret = vkGetSwapchainImagesKHR(mDevice->GetDevice(), mSwapchain, &swapchainImageCount, mImages.data());

        return ret == VK_SUCCESS;
    }

    void SpVKSwapchain::QuerySurfaceCapabilities() {
        //capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mContext->GetPhysicalDevice(), mContext->GetSurface(), &mSurfaceInfo.capabilities);

        SpVkDeviceSettings settings = mDevice->GetSettings();

        //format
        uint32_t formatCount;
        CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(mContext->GetPhysicalDevice(), mContext->GetSurface(), &formatCount, nullptr));
        if(formatCount == 0){
            SpLogError("{0}, num of surface format is 0", __FUNCTION__);
            return;
        }
        VkSurfaceFormatKHR formats[formatCount];
        CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(mContext->GetPhysicalDevice(), mContext->GetSurface(), &formatCount, formats));
        uint32_t formatIndex = -1;
        for(int i = 0; i < formatCount; i++){
            if(formats[i].format == settings.surfaceFormat && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
                formatIndex = i;
                break;
            }
        }
        if(formatIndex == -1){
            formatIndex = 0;
        }
        mSurfaceInfo.surfaceFormat = formats[formatIndex];

        //present mode
        uint32_t presentModeCount;
        CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(mContext->GetPhysicalDevice(), mContext->GetSurface(), &presentModeCount,
                                                          nullptr));
        if(presentModeCount == 0){
            SpLogError("{0}, number of surface present mode is 0.", __FUNCTION__);
            return;
        }
        VkPresentModeKHR presentModes[presentModeCount];
        CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(mContext->GetPhysicalDevice(), mContext->GetSurface(), &presentModeCount,
                                                          presentModes));
        uint32_t presentModeIndex = -1;
        for(int i = 0; i < presentModeCount; i++){
            if(presentModes[i] == settings.presentMode){
                presentModeIndex = i;
                break;
            }
        }
        if(presentModeIndex == -1){
            presentModeIndex = 0;
        }
        mSurfaceInfo.presentMode = presentModes[presentModeIndex];
    }

}