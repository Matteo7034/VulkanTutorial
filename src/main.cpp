#include <vulkan/vulkan.h>
//
#include "../include/fun.h"
#include <cstdlib>
#include <iostream>
// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main() {
  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
