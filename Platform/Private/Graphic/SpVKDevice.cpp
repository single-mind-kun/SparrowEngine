#include "Graphic/SpVkDevice.h"
#include "Graphic/SpVKGraphicContext.h"
#include "Graphic/SpVKQueue.h"

namespace spe{
    const DeviceFeature requestExtensions[] = {
            {VK_KHR_SWAPCHAIN_EXTENSION_NAME, true},
#ifdef SP_ENGINE_PLATFORM_WIN32
#elif SP_ENGINE_PLATFORM_MACOS
            { "VK_KHR_portability_subset", true },
#elif SP_ENGINE_PLATFORM_LINUX
            { VK_KHR_XCB_SURFACE_EXTENSION_NAME, true },
#endif
};

    SpVKDevice::SpVKDevice(SpVKGraphicContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount,
                           const SpVkDeviceSettings &settings) : mSettings(settings){
        if(!context){
            SpLogError("Must create a vulkan graphic context before create device.");
            return;
        }

        QueueFamilyInfo graphicQueueFamilyInfo = context->GetGraphicQueueFamilyInfo();
        QueueFamilyInfo presentQueueFamilyInfo = context->GetPresentQueueFamilyInfo();

        if(graphicQueueFamilyInfo.queueCount < graphicQueueCount){
            SpLogError("this queue family has {0} queue count, but request {1}", graphicQueueFamilyInfo.queueCount, graphicQueueCount);
            return;
        }

        if(presentQueueFamilyInfo.queueCount < presentQueueCount){
            SpLogError("this queue family has {0} queue count, but request {1}", presentQueueFamilyInfo.queueCount, presentQueueCount);
            return;
        }

        std::vector<float> graphicQueuePriorities(graphicQueueCount, 0.0f);
        std::vector<float> presentQueuePriorities(presentQueueCount, 1.0f);

        bool bSameQueueFamilyIndex = context->IsSameGraphicPresentQueueFamily();
        uint32_t sameQueueCount = graphicQueueCount;
        if(bSameQueueFamilyIndex){
            sameQueueCount += presentQueueCount;
            if(sameQueueCount > graphicQueueFamilyInfo.queueCount){
                sameQueueCount = graphicQueueFamilyInfo.queueCount;
            }
            graphicQueuePriorities.insert(graphicQueuePriorities.end(), presentQueuePriorities.begin(), presentQueuePriorities.end());
        }

        VkDeviceQueueCreateInfo queueInfos[2] = {
                {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .queueFamilyIndex = static_cast<uint32_t>(graphicQueueFamilyInfo.queueFamilyIndex),
                    .queueCount = sameQueueCount,
                    .pQueuePriorities = graphicQueuePriorities.data()
                }
        };

        if(!bSameQueueFamilyIndex){
            queueInfos[1] = {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .queueFamilyIndex = static_cast<uint32_t>(presentQueueFamilyInfo.queueFamilyIndex),
                    .queueCount = presentQueueCount,
                    .pQueuePriorities = presentQueuePriorities.data()
            };
        }

        uint32_t availableExtensionCount;
        CALL_VK(vkEnumerateDeviceExtensionProperties(context->GetPhysicalDevice(),"",&availableExtensionCount, nullptr));
        VkExtensionProperties availableExtensions[availableExtensionCount];
        CALL_VK(vkEnumerateDeviceExtensionProperties(context->GetPhysicalDevice(), "", &availableExtensionCount, availableExtensions));
        uint32_t enableExtensionCount;
        const char* enableExtensions[32];
        if(!checkDeviceFeatures("Device Extension", true, availableExtensionCount, availableExtensions,
                ARRAY_SIZE(requestExtensions), requestExtensions, &enableExtensionCount, enableExtensions)){
            return;
        }

        VkDeviceCreateInfo deviceInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueCreateInfoCount = static_cast<uint32_t>(bSameQueueFamilyIndex ? 1 : 2),
                .pQueueCreateInfos = queueInfos,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = enableExtensionCount,
                .ppEnabledExtensionNames = enableExtensionCount > 0 ? enableExtensions : nullptr,
                .pEnabledFeatures = nullptr
        };
        CALL_VK(vkCreateDevice(context->GetPhysicalDevice(),&deviceInfo,nullptr, &mDevice));
        SpLogTrace("VkDevice: {0}", (void*)mDevice);

        for(int i = 0; i < graphicQueueCount; i++){
            VkQueue queue;
            vkGetDeviceQueue(mDevice, graphicQueueFamilyInfo.queueFamilyIndex, i, &queue);
            mGraphicQueues.push_back(std::make_shared<SpVKQueue>(graphicQueueFamilyInfo.queueFamilyIndex, i, queue, false));
        }

        for(int i = 0; i < presentQueueCount; i++){
            VkQueue queue;
            vkGetDeviceQueue(mDevice, presentQueueFamilyInfo.queueFamilyIndex, i, &queue);
            mPresentQueues.push_back(std::make_shared<SpVKQueue>(presentQueueFamilyInfo.queueFamilyIndex, i, queue, true));
        }

    }


    SpVKDevice::~SpVKDevice() {
        vkDeviceWaitIdle(mDevice);
        vkDestroyDevice(mDevice, nullptr);
    }
}