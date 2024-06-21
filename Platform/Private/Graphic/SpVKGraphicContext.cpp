#include "Graphic/SpVKGraphicContext.h"
#include "Window/SpGLFWwindow.h"
#include "GLFW/glfw3.h"

namespace spe{
    //配置验证层
    const DeviceFeature requestedLayers[] = {
            {"VK_LAYER_KHRONOS_validation", true}
    };

    //配置Extension
    const DeviceFeature requestExtensions[] = {
            { VK_KHR_SURFACE_EXTENSION_NAME, true},
            {VK_EXT_DEBUG_REPORT_EXTENSION_NAME, true},
#ifdef SP_ENGINE_PLATFORM_WIN32
            { VK_KHR_WIN32_SURFACE_EXTENSION_NAME, true },
#elif SP_ENGINE_PLATFORM_MACOS
            { VK_MVK_MACOS_SURFACE_EXTENSION_NAME, true },
#elif SP_ENGINE_PLATFORM_LINUX
            { VK_KHR_XCB_SURFACE_EXTENSION_NAME, true },
#endif
    };

    SpVKGraphicContext::SpVKGraphicContext(SpWindow *window) {
        CreateInstance();
        CreateSurface(window);
        SelectPhyDevice();
    }

    SpVKGraphicContext::~SpVKGraphicContext() {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        vkDestroyInstance(mInstance, nullptr);
    }

    static VkBool32 VkDebugReportCallback(VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT                  objectType,
                uint64_t                                    object,
                size_t                                      location,
                int32_t                                     messageCode,
                const char*                                 pLayerPrefix,
                const char*                                 pMessage,
                void*                                       pUserData){
        if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT){
            SpLogError("{0}", pMessage);
        }
        if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT){
            SpLogWarning("{0}", pMessage);
        }
        return VK_TRUE;
    }

    void SpVKGraphicContext::CreateInstance() {
        //1. 构建layers
        uint32_t availableLayerCount;
        CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr));
        VkLayerProperties availableLayers[availableLayerCount];
        CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

        uint32_t enableLayerCount = 0;
        const char* enableLayers[32];
        if(bShouldValidate){
            if(!checkDeviceFeatures("Instance Layers", false, availableLayerCount, availableLayers,
                                    ARRAY_SIZE(requestedLayers), requestedLayers,&enableLayerCount,enableLayers)){
                return;
            }
        }


        //2. 构建extension
        uint32_t availableExtensionCount;
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, nullptr));
        VkExtensionProperties availableExtension[availableExtensionCount];
        CALL_VK(vkEnumerateInstanceExtensionProperties("", &availableExtensionCount, availableExtension));

        uint32_t glfwRequestedExtensionCount;
        const char** glfwRequestedExtension = glfwGetRequiredInstanceExtensions(&glfwRequestedExtensionCount);
        std::unordered_set<const char*> allRequestedExtensionSet;
        std::vector<DeviceFeature> allRequestedExtensions;

        //添加自定义扩展
        for(const auto& request : requestExtensions){
            if(allRequestedExtensionSet.find(request.name) == allRequestedExtensionSet.end()){}
            allRequestedExtensionSet.insert(request.name);
            allRequestedExtensions.push_back(request);
        }

        //添加glfw必须的扩展
        for(int i = 0; i < glfwRequestedExtensionCount; i++) {
            const char* extensionName = glfwRequestedExtension[i];
            if(allRequestedExtensionSet.find(extensionName) == allRequestedExtensionSet.end()){
                allRequestedExtensionSet.insert(extensionName);
                allRequestedExtensions.push_back({extensionName, true});
            }
        }

        uint32_t enableExtensionCount;
        const char* enableExtensions[32];
        if(!checkDeviceFeatures("Instance Extension", true, availableExtensionCount, availableExtension,
                                allRequestedExtensions.size(), allRequestedExtensions.data(),&enableExtensionCount,enableExtensions)){
            return;
        }

        VkApplicationInfo applicationInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Sparrow_Engine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "None",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3
        };

        VkDebugReportCallbackCreateInfoEXT debugReportCallbackInfoExt = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
                .pNext = nullptr,
                .flags = VK_DEBUG_REPORT_WARNING_BIT_EXT
                        | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
                        | VK_DEBUG_REPORT_ERROR_BIT_EXT,
                .pfnCallback = VkDebugReportCallback
        };
        //3. create instance

        VkInstanceCreateInfo instanceInfo = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext = bShouldValidate ? &debugReportCallbackInfoExt : nullptr,
                .flags = 0,
                .pApplicationInfo = &applicationInfo,
                .enabledLayerCount = enableLayerCount,
                .ppEnabledLayerNames = enableLayerCount > 0 ? enableLayers : nullptr,
                .enabledExtensionCount = enableExtensionCount,
                .ppEnabledExtensionNames = enableExtensionCount > 0 ? enableExtensions : nullptr
        };
        CALL_VK(vkCreateInstance(&instanceInfo, nullptr,&mInstance));

        SpLogTrace("{0} : Instance : {1}", __FUNCTION__, (void*)mInstance);
    }

    void SpVKGraphicContext::CreateSurface(SpWindow* window) {
        if(!window){
            SpLogError("window is not exist.");
            return;
        }

        //可以转成其他窗口，目前不符合开闭原则 TODO
        SpGLFWwindow* glfwWindow = dynamic_cast<SpGLFWwindow*>(window);
        if(!glfwWindow){
            SpLogError("glfw window is not exist.");
            return;
        }

        CALL_VK(glfwCreateWindowSurface(mInstance, glfwWindow->GetWindowHandle(), nullptr, &mSurface));
        SpLogTrace("{0} : surface : {1}", __FUNCTION__, (void*)mSurface);
    }

    void SpVKGraphicContext::SelectPhyDevice() {
        uint32_t phyDeviceCount;
        CALL_VK(vkEnumeratePhysicalDevices(mInstance, &phyDeviceCount, nullptr));
        VkPhysicalDevice phyDevices[phyDeviceCount];
        CALL_VK(vkEnumeratePhysicalDevices(mInstance, &phyDeviceCount, phyDevices));

        uint32_t maxScore = 0;
        uint32_t maxScorePhyDeviceIndex = 0;
        SpLogDebug("-----------------------------");
        SpLogDebug("Physical devices: ");
        for(int i = 0; i < phyDeviceCount; i++){
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(phyDevices[i], &props);
            PrintPhyDeviceInfo(props);

            uint32_t score = GetPhyDeviceScore(props);
            uint32_t formatCount;
            CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevices[i], mSurface, &formatCount,nullptr));
            VkSurfaceFormatKHR formats[formatCount];
            CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevices[i], mSurface, &formatCount,formats));
            for(int j = 0; j < formatCount; j++){
                if(formats[j].format == VK_FORMAT_B8G8R8A8_UNORM && formats[j].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR){
                    score += 10;
                    break;
                }
            }


            //query queue family
            uint32_t queueFamilyCount;
            vkGetPhysicalDeviceQueueFamilyProperties(phyDevices[i],&queueFamilyCount, nullptr);
            VkQueueFamilyProperties queueFamilys[queueFamilyCount];
            vkGetPhysicalDeviceQueueFamilyProperties(phyDevices[i],&queueFamilyCount, queueFamilys);

            SpLogDebug("score    --->    : {0}", score);
            SpLogDebug("queue family     : {0}", queueFamilyCount);
            if(score < maxScore){
                continue;
            }

            for(int j = 0; j < queueFamilyCount; j++){
                if(queueFamilys[j].queueCount == 0){
                    continue;
                }

                //1. graphic family
                if(queueFamilys[j].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                    mGraphicQueueFamily.queueFamilyIndex = j;
                    mGraphicQueueFamily.queueCount = queueFamilys[j].queueCount;
                }

                if(mGraphicQueueFamily.queueFamilyIndex >= 0 && mPresentQueueFamily.queueFamilyIndex >= 0
                    && mGraphicQueueFamily.queueFamilyIndex != mPresentQueueFamily.queueFamilyIndex){
                    break;
                }

                //2. present family
                VkBool32 bSupportSurface;
                vkGetPhysicalDeviceSurfaceSupportKHR(phyDevices[i], j, mSurface, &bSupportSurface);
                if(bSupportSurface){
                    mPresentQueueFamily.queueFamilyIndex = j;
                    mPresentQueueFamily.queueCount = queueFamilys[j].queueCount;
                }
            }

            if(mGraphicQueueFamily.queueFamilyIndex >= 0 && mPresentQueueFamily.queueFamilyIndex >= 0){
                maxScore = score;
                maxScorePhyDeviceIndex = i;
            }
        }
        SpLogDebug("-----------------------------");
        if(maxScorePhyDeviceIndex < 0){
            SpLogWarning("Maybe can not find a suitable physic device, will 0.");
            maxScorePhyDeviceIndex = 0;
        }

        mPhysicalDevice = phyDevices[maxScorePhyDeviceIndex];
        vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mPhysicalDeviceMemoryProperties);
        SpLogTrace("{0} : physical device:{1}, score:{2}, graphic queue: {3} : {4}, present queue: {5} : {6}", __FUNCTION__, maxScorePhyDeviceIndex, maxScore,
              mGraphicQueueFamily.queueFamilyIndex, mGraphicQueueFamily.queueCount,
              mPresentQueueFamily.queueFamilyIndex, mPresentQueueFamily.queueCount);
    }

    void SpVKGraphicContext::PrintPhyDeviceInfo(VkPhysicalDeviceProperties &props) {
        //    uint32_t                            apiVersion;
        //    uint32_t                            driverVersion;
        //    uint32_t                            vendorID;
        //    uint32_t                            deviceID;
        //    VkPhysicalDeviceType                deviceType;
        //    char                                deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
        //    uint8_t                             pipelineCacheUUID[VK_UUID_SIZE];
        //    VkPhysicalDeviceLimits              limits;
        //    VkPhysicalDeviceSparseProperties    sparseProperties;
        const char *deviceType = props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "integrated gpu" :
                                 props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "discrete gpu" :
                                 props.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "virtual gpu" :
                                 props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "cpu" : "others";

        uint32_t driverVersionMajor = VK_VERSION_MAJOR(props.driverVersion);
        uint32_t driverVersionMinor = VK_VERSION_MINOR(props.driverVersion);
        uint32_t driverVersionPatch = VK_VERSION_PATCH(props.driverVersion);

        uint32_t apiVersionMajor = VK_VERSION_MAJOR(props.apiVersion);
        uint32_t apiVersionMinor = VK_VERSION_MINOR(props.apiVersion);
        uint32_t apiVersionPatch = VK_VERSION_PATCH(props.apiVersion);

        SpLogDebug("-----------------------------");
        SpLogDebug("deviceName       : {0}", props.deviceName);
        SpLogDebug("deviceType       : {0}", deviceType);
        SpLogDebug("vendorID         : {0}", props.vendorID);
        SpLogDebug("deviceID         : {0}", props.deviceID);
        SpLogDebug("driverVersion    : {0}.{1}.{2}", driverVersionMajor, driverVersionMinor, driverVersionPatch);
        SpLogDebug("apiVersion       : {0}.{1}.{2}", apiVersionMajor, apiVersionMinor, apiVersionPatch);
    }

    uint32_t SpVKGraphicContext::GetPhyDeviceScore(VkPhysicalDeviceProperties &props) {
        VkPhysicalDeviceType deviceType = props.deviceType;
        uint32_t score = 0;
        switch(deviceType){
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += 40;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score +=30;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                score += 20;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += 10;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
                break;
        }
        return score;
    }
}