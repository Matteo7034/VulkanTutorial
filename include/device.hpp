#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <optional>
#include "../include/debug.hpp"
namespace VkDevices {

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance); 
    
    int rateDeviceSuitability(VkPhysicalDevice device);
    
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() const {
            return graphicsFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    
    VkDevice createLogicalDevice(VkPhysicalDevice physicaldevice,VkQueue& graphicsQueue);
}
