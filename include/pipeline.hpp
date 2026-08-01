#pragma once
#include <vector>
#include <fstream>
#include <vulkan/vulkan.h>
#include <iostream>
#include "../include/swapchain.hpp"
namespace PipeLine{
    void createGraphicsPipeline(
            VkDevice device,
            VkExtent2D swapChainExtent,
            VkPipelineLayout& pipelineLayout,
            VkRenderPass renderPass,
            VkPipeline& graphicsPipeline);
    
    std::vector<char> readFile(const std::string& filename);

    VkShaderModule createShaderModule(
            const std::vector<char>& code,
            VkDevice device);

}//namespace PipeLine
