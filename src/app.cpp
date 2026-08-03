#include "../include/app.hpp"
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
        renderPass = SwapChain::createRenderPass(
                device,
                swapChainImageFormat
                );
        PipeLine::createGraphicsPipeline(
                device,
                swapChainExtent,
                pipelineLayout,
                renderPass,
                graphicsPipeline);
        swapChainFramebuffers = SwapChain::createFramebuffers(
            device,
            renderPass,
            swapChainImageViews,
            //swapChainFramebuffers,
            swapChainExtent);
        commandPool = Commands::
            createCommandPool(device,physicalDevice,surface);
        commandBuffers = Commands::
            createCommandBuffers(device,commandPool,renderPass,MAX_FRAMES_IN_FLIGHT);
        createSyncObjects();
    }
    
    void HelloTriangleApplication::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(device);
    }

    void HelloTriangleApplication::cleanup() {
        std::cout<<"[INFO] cleaning up...\n";
        for(size_t i = 0; i<MAX_FRAMES_IN_FLIGHT;i++){
            vkDestroySemaphore(device,imageAvailableSemaphores[i],nullptr);
            vkDestroySemaphore(device,renderFinishedSemaphores[i],nullptr);
            vkDestroyFence(device,inFlightFences[i],nullptr);
        }
        vkDestroyCommandPool(device, commandPool, nullptr);
        //vkDestroyPipeline(device, graphicsPipeline, nullptr);
        for (auto framebuffer : swapChainFramebuffers) {
           vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
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
    
    void HelloTriangleApplication::drawFrame(){
        vkWaitForFences(device,1,&inFlightFences[currentFrame],VK_TRUE,UINT64_MAX);
        vkResetFences(device,1,&inFlightFences[currentFrame]);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(
                device,
                swapChain,
                UINT64_MAX,
                imageAvailableSemaphores[currentFrame],
                VK_NULL_HANDLE,
                &imageIndex);
        
        vkResetCommandBuffer(commandBuffers[currentFrame],0);
        Commands::recordCommandBuffer(
                commandBuffers[currentFrame],
                imageIndex,
                renderPass,
                swapChainFramebuffers,
                swapChainExtent,
                graphicsPipeline);
        //Submitting the command buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        if(vkQueueSubmit(graphicsQueue,1,&submitInfo,inFlightFences[currentFrame])!= VK_SUCCESS){
            throw std::runtime_error("failed to submit draw command buffer!");
        }
        VkPresentInfoKHR presentInfo {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;
        vkQueuePresentKHR(presentQueue,&presentInfo);
        currentFrame = (currentFrame + 1 ) % MAX_FRAMES_IN_FLIGHT;
    }

    void HelloTriangleApplication::createSyncObjects(){
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
       for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || 
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        } 
    }
} //namespcae App
