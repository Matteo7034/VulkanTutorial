#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

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
        VkInstance instance; 
        GLFWwindow *window;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkQueue graphicsQueue;    
        VkSurfaceKHR surface;
        VkQueue presentQueue;
        VkSwapchainKHR swapChain;
    };
}// namespace App
