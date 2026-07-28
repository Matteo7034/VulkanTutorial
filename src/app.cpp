#include "../include/app.hpp"
#include "../include/debug.hpp"
#include "../include/init.hpp"
#include "../include/device.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
namespace App{
    void HelloTriangleApplication::run() {
      initWindow();
      initVulkan();
      mainLoop();
      cleanup();
    }

    void HelloTriangleApplication::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
    }


    void HelloTriangleApplication::initVulkan() {
        // Inizializzo vulkan...
        std::cout << "Vulkan init..." << std::endl;
  
        instance = VulkanInit::createInstance();
        VulkanDebug::setupDebugMessager(instance,debugMessenger);
        createSurface(); 
        physicalDevice = VkDevices::pickPhysicalDevice(instance,surface);
        device = VkDevices::createLogicalDevice(
                physicalDevice,
                surface,
                graphicsQueue,
                presentQueue
                );
    }
    
    void HelloTriangleApplication::mainLoop() {
      while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
      }
    }

    void HelloTriangleApplication::cleanup() {
        if(device != VK_NULL_HANDLE){
            vkDestroyDevice(device,nullptr);
            device = VK_NULL_HANDLE;
        }
        if(enableValidationLayers)
        {
            VulkanDebug::DestroyDebugUtilsMessengerEXT(instance,debugMessenger,nullptr);
       }
        vkDestroySurfaceKHR(instance,surface,nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void HelloTriangleApplication::createSurface(){
        if(glfwCreateWindowSurface(instance,window,nullptr,&surface)!=VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }
} //namespcae App
