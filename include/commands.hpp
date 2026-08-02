#pragma once
#include <vulkan/vulkan.h>
#include "../include/device.hpp"
namespace Commands{
    VkCommandPool createCommandPool(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkSurfaceKHR surface);

    VkCommandBuffer createCommandBuffer(
           VkDevice device,
           VkCommandPool commandPool,
           VkRenderPass renderPass
           );


    void recordCommandBuffer(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex,
            VkRenderPass renderPass,
            const std::vector<VkFramebuffer>& swapChainFramebuffers,
            VkExtent2D swapChainExtent,
            VkPipeline graphicsPipeline
            );
}
