
#include "../include/device.hpp"


namespace VkDevices{

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance,VkSurfaceKHR surface)
    {
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount =0;
        vkEnumeratePhysicalDevices(instance,&deviceCount,nullptr);
        if(deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance,&deviceCount,devices.data());
        
        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, VkPhysicalDevice> candidates;
        
        for ( const auto& device : devices )
        {
            /*
            if(isDeviceSuitable(device)){
                physicalDevice = device;
                break;
            }
            */
            int score = rateDeviceSuitability(device,surface);
            candidates.insert(std::make_pair(score, device));
        }
        /*
        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }*/
        if (candidates.rbegin()->first > 0) {
            physicalDevice = candidates.rbegin()->second;
        } else {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        return physicalDevice;
    }
    bool checkDeviceExtensionSupport(VkPhysicalDevice device){
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device,nullptr, &extensionCount,nullptr);
        std::vector<VkExtensionProperties> aviableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,aviableExtensions.data());
        std::set<std::string> requiredExtensions(deviceExtensions.begin(),deviceExtensions.end());
        for(const auto& extension : aviableExtensions){
            requiredExtensions.erase(extension.extensionName);
        }
        bool supported = requiredExtensions.empty();
        if(supported){
            std::cout<<"[INFO] GPU extension supported: VK_KHR_swapchain\n";
        }else{
            std::cout<<"[Error] GPU extension NOT supported: VK_KHR_swapchain\n";
        }
        return supported;
    }
       
    int rateDeviceSuitability(VkPhysicalDevice device,VkSurfaceKHR surface){
        
        QueueFamilyIndices indices = findQueueFamilies(device,surface); 
        if (!indices.isComplete()) return 0;
        //extensionSupport
        bool extensionsSupported = checkDeviceExtensionSupport(device); 
        if (!extensionsSupported) return 0; 
        //swapchain
        bool swapChainAdequate = false;
        if(extensionsSupported)
        {
            SwapChain::SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(device,surface);
            swapChainAdequate = 
                !swapChainSupport.formats.empty() &&
                !swapChainSupport.presentModes.empty();
        }
        if(!swapChainAdequate) return 0;
        // Properties:
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device,&deviceProperties);
        // Features:
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device,&deviceFeatures);
        
        int score = 0;
        
        if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        score += deviceProperties.limits.maxImageDimension2D; 
        
        if ( !deviceFeatures.geometryShader){
            return 0;
        }
        std::cout<<"[INFO] Device Suitability score: "<<score<<"\n";        
       // return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
        return score;
    }
    

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,VkSurfaceKHR surface){
        QueueFamilyIndices indices;
        
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies.data());
        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                indices.presentFamily = i;
            }
            // Se abbiamo trovato tutto quello che ci serve, usciamo subito dal cicl
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }
    
    VkDevice createLogicalDevice(
            VkPhysicalDevice physicalDevice,
            VkSurfaceKHR surface,
            VkQueue& graphicsQueue,
            VkQueue& presentQueue
            )
    {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice,surface);
       
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        
        std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
        };
        
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames=deviceExtensions.data(); 

        if(enableValidationLayers){
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else{
            createInfo.enabledLayerCount = 0;
        }
        VkDevice device;

        if (vkCreateDevice(physicalDevice,&createInfo,nullptr, &device) != VK_SUCCESS){
            throw std::runtime_error("failed to create logical device !");
        }
        vkGetDeviceQueue(device,indices.graphicsFamily.value(), 0 ,&graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
        return device;
    }
}






