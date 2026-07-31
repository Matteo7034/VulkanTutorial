#pragma once
#include <vector>
#include <fstream>
#include <vulkan/vulkan.h>
#include <iostream>
namespace PipeLine{
    void createGraphicsPipeline(VkDevice device);
    std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(
            const std::vector<char>& code,VkDevice device);

}//namespace PipeLine
