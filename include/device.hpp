#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <optional>
#include "../include/debug.hpp"
#include <set>
namespace VkDevices {

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance,VkSurfaceKHR surface); 
    
    int rateDeviceSuitability(VkPhysicalDevice device,VkSurfaceKHR surface);
    
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,VkSurfaceKHR surface);
    
    VkDevice createLogicalDevice(
            VkPhysicalDevice physicaldevice,
            VkSurfaceKHR surface,
            VkQueue& graphicsQueue,
            VkQueue& presentQueue);
}
