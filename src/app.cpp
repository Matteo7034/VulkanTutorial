#include "../include/app.hpp"
#include "../include/debug.hpp"
#include "../include/init.hpp"
#include "../include/device.hpp"
#include "../include/pipeline.hpp"
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
        std::cout << "[INFO] Init Vulkan creating instance..." << std::endl;
  
        instance = VulkanInit::createInstance();
        VulkanDebug::setupDebugMessager(instance,debugMessenger);
        createSurface(); 
        std::cout<<"[INFO] picking physical device\n";
        physicalDevice = VkDevices::pickPhysicalDevice(instance,surface);
        device = VkDevices::createLogicalDevice(
                physicalDevice,
                surface,
                graphicsQueue,
                presentQueue
                );
        createSwapChain();
        createImageViews();
        std::cout<<"[INFO] creating Graphics pipeline\n";
        PipeLine::createGraphicsPipeline(device,swapChainExtent,pipelineLayout);
    }
    
    void HelloTriangleApplication::mainLoop() {
      while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
      }
    }

    void HelloTriangleApplication::cleanup() {
        std::cout<<"[INFO] cleaning up...\n";
        //vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        for(auto imageView : swapChainImageViews){
            vkDestroyImageView(device,imageView,nullptr);
        }
        if(swapChain != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(device,swapChain,nullptr);
        if(device != VK_NULL_HANDLE){
            vkDestroyDevice(device,nullptr);
            device = VK_NULL_HANDLE;
        }
        if(enableValidationLayers)
        {
            VulkanDebug::DestroyDebugUtilsMessengerEXT(
                    instance,
                    debugMessenger,
                    nullptr);
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
    void HelloTriangleApplication::createSwapChain()
    {
        SwapChain::SwapChainSupportDetails swapChainSupport =
            SwapChain::querySwapChainSupport(physicalDevice,surface);

        VkSurfaceFormatKHR surfaceFormat = 
            SwapChain::chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = 
            SwapChain::chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = 
            SwapChain::chooseSwapExtent(swapChainSupport.capabilities,window);
        
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if(swapChainSupport.capabilities.maxImageCount > 0 &&
                imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VkDevices::QueueFamilyIndices indices = 
            VkDevices::findQueueFamilies(physicalDevice,surface);
        uint32_t queueFamilyIndices[] = 
        { indices.graphicsFamily.value(), indices.presentFamily.value()};

        if(indices.graphicsFamily != indices.presentFamily){
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }else{
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;
        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain)
                != VK_SUCCESS){
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(device,swapChain,&imageCount,nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device,swapChain,&imageCount,swapChainImages.data());
        
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }
    void HelloTriangleApplication::createImageViews(){
        swapChainImageViews = SwapChain::createImageViews(
                device,
                swapChainImages,
                swapChainImageFormat);
    }
} //namespcae App
