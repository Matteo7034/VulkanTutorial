#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <optional>
#include "debug.hpp"
#include <set>
#include "swapchain.hpp"
namespace VkDevices {

    //needed for swapchain 
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance,VkSurfaceKHR surface); 
    
    int rateDeviceSuitability(VkPhysicalDevice device,VkSurfaceKHR surface);
    
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

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
