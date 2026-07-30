#include "../include/pipeline.hpp"

namespace PipeLine{
    std::vector<char> readFile(const std::string& filename){
        std::ifstream file(filename,std::ios::ate | std::ios::binary);
        if(!file.is_open()){
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(),fileSize);
        file.close();
        return buffer;
    }
    void createGraphicsPipeline(VkDevice device){
        auto vertShaderCode = readFile("shaders/vert.spv");
        auto fragShaderCode = readFile("shaders/frag.spv");
        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode,device);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode,device);
        vkDestroyShaderModule(device,fragShaderModule,nullptr);
        vkDestroyShaderModule(device,vertShaderModule,nullptr);
    }
    VkShaderModule createShaderModule(const std::vector<char>& code,VkDevice device){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        VkShaderModule shaderModule;
        if(vkCreateShaderModule(device,&createInfo,nullptr,&shaderModule) !=
                VK_SUCCESS){
            throw std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }
}//namespace Pipeline
