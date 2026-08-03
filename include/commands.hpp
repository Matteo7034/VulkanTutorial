#pragma once
#include <vulkan/vulkan.h>
#include "device.hpp"
#include "app.hpp"
namespace Commands{
    VkCommandPool createCommandPool(
            VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkSurfaceKHR surface);

    std::vector<VkCommandBuffer> createCommandBuffers(
           VkDevice device,
           VkCommandPool commandPool,
           VkRenderPass renderPass,
           uint32_t count
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
