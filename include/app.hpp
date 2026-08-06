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
constexpr int MAX_FRAMES_IN_FLIGHT = 2; // Add synchronization count


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
        void drawFrame();
        void createSyncObjects();
        void recreateSwapChain();
        void cleanupSwapChain();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
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
        std::vector<VkCommandBuffer> commandBuffers;
        //Syncronization Objects
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        bool framebufferResized = false;
        uint32_t currentFrame = 0;

    };
}// namespace App
