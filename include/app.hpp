#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "debug.hpp"
#include "init.hpp"
#include "device.hpp"
#include "pipeline.hpp"
#include "commands.hpp"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;



namespace App { 

    class HelloTriangleApplication {
    public:
      void run();

    private:
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();
        void createInstance();
        void createSurface();
        void createSwapChain();
        void createImageViews();
        VkInstance instance; 
        GLFWwindow *window;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkQueue graphicsQueue;    
        VkSurfaceKHR surface;
        VkQueue presentQueue;
        //SwapChain
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;
        //pipeline
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        //Command Buffer
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
    };
}// namespace App
