
#include "../include/device.hpp"


namespace VkDevices{

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance)
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
            int score = rateDeviceSuitability(device);
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

    int rateDeviceSuitability(VkPhysicalDevice device){
        
        QueueFamilyIndices indices = findQueueFamilies(device);
        
        if (!indices.isComplete()){
            return 0;
        }
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
        
       // return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
        return score;
    }
    

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device){
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
            i++;
            // Se abbiamo trovato tutto quello che ci serve, usciamo subito dal ciclo!
            if (indices.isComplete()) {
                break;
            }
        }
        return indices;
    }
    VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice,VkQueue& graphicsQueue){
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;
       
        createInfo.enabledExtensionCount = 0;

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
        return device;
    }
}






