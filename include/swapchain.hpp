#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <GLFW/glfw3.h>
#include "app.hpp"

namespace SwapChain{
    struct SwapChainSupportDetails{
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(
            VkPhysicalDevice device,
            VkSurfaceKHR surface);
    
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);
    
    VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);
    
    VkExtent2D chooseSwapExtent(
            const VkSurfaceCapabilitiesKHR& capabilities,
            GLFWwindow* window);

    std::vector<VkImageView> createImageViews(
            VkDevice device,
            const std::vector<VkImage>& swapChainImages,
            VkFormat swapChainImageFormat
            );

    VkRenderPass createRenderPass(
            VkDevice device,
            VkFormat swapChainImageFormat
            );
    
    std::vector<VkFramebuffer> createFramebuffers(
            VkDevice device,
            VkRenderPass renderPass,
            const std::vector<VkImageView>& swapChainImageViews,
            //std::vector<VkFramebuffer>& swapChainFramebuffers,
            VkExtent2D swapChainExtent
            );
}//namespace SwapChain
