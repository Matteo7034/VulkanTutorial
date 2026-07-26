#pragma once

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
        VkInstance instance; 
        GLFWwindow *window;
        void pickPhysicalDevice();
    };
}// namespace App
