#include "fun.h"

#include <iostream>
#include <vulkan/vulkan_core.h>

void HelloTriangleApplication::createInstance() {
    // Check Validation Layers
    if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error(
        "Validation layers requested, but not avaiable!\n");
    }
    // Application Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Instance Create Info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // GLFW extensions
    uint32_t glfwExtensionCount = 0;
    auto extensions = getRequiredExtentions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // VKCreateInfo
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // Creazione Istanza Vulkan
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    // check
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    // Debug extensions aviabile
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> aviabileextensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         aviabileextensions.data());
    std::cout << "available extensions:\n";

    for (const auto &extension : aviabileextensions) {
    std::cout << '\t' << extension.extensionName << '\n';
    }
}
