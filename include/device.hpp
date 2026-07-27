#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <map>

namespace VkDevices {

    void pickPhysicalDevice(VkInstance instance); 
    
    int rateDeviceSuitability(VkPhysicalDevice device);
}
