
#include "../include/device.hpp"


namespace VkDevices{

    void pickPhysicalDevice(VkInstance instance){
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount =0;
        vkEnumeratePhysicalDevices(instance,&deviceCount,nullptr);
        if(deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        //std::vector
    }
}
