#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <optional>

namespace VkDevices {

    void pickPhysicalDevice(VkInstance instance); 
    
    int rateDeviceSuitability(VkPhysicalDevice device);
    
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete(){
            return graphicsFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

}
