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

        VkInstance GetInstance() const {return mInstance;}
        VkSurfaceKHR GetSurface() const {return mSurface;}
        VkPhysicalDevice GetPhysicalDevice() const {return mPhysicalDevice;}
        const QueueFamilyInfo GetGraphicQueueFamilyInfo() const {return mGraphicQueueFamily;}
        const QueueFamilyInfo GetPresentQueueFamilyInfo() const {return mPresentQueueFamily;}
        VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties () const {return mPhysicalDeviceMemoryProperties;}
        bool IsSameGraphicPresentQueueFamily() const {return mGraphicQueueFamily.queueFamilyIndex == mPresentQueueFamily.queueFamilyIndex;}


    private:
        void PrintPhyDeviceInfo(VkPhysicalDeviceProperties& props);
        uint32_t GetPhyDeviceScore(VkPhysicalDeviceProperties &props);


        void CreateInstance();
        void CreateSurface(SpWindow* window);
        void SelectPhyDevice();

        bool bShouldValidate = true;
        VkInstance mInstance = VK_NULL_HANDLE;
        VkSurfaceKHR mSurface = VK_NULL_HANDLE;

        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
        QueueFamilyInfo mGraphicQueueFamily;
        QueueFamilyInfo mPresentQueueFamily;

    };
}

#endif