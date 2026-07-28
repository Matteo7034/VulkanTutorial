#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

extern const std::vector<const char *> validationLayers;

namespace VulkanDebug {
    
    
    bool checkValidationLayerSupport();
    
    std::vector<const char *> getRequiredExtentions();
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL 
        debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                void *pUserData);
    
    
    void setupDebugMessager(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger);
    
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);
    
    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessager,
            const VkAllocationCallbacks* pAllocator);
    
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);



}

