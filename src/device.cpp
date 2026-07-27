
#include "../include/device.hpp"


namespace VkDevices{

    void pickPhysicalDevice(VkInstance instance)
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

}
