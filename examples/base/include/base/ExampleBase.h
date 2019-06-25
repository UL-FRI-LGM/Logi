#ifndef BASE_EXAMPLE_BASE_H
#define BASE_EXAMPLE_BASE_H

#define GLFW_INCLUDE_VULKAN
#include <base/GLFWManager.h>
#include <iostream>
#include "logi/logi.hpp"

struct ExampleConfiguration {
  std::string windowTitle = "Example";
  int32_t windowWidth = 800;
  int32_t windowHeight = 600;
  size_t maxFramesInFlight = 2u;
  std::vector<const char*> instanceExtensions;
  std::vector<const char*> deviceExtensions;
  std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
};

class ExampleBase {
 public:
  explicit ExampleBase(const ExampleConfiguration& config = {});

  void run();

 protected:
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj_type,
                                                      uint64_t obj, size_t location, int32_t code,
                                                      const char* layer_prefix, const char* msg, void* user_data);

  void initWindow();

  void createInstance();

  void initSurface();

  void selectDevice();

  void initializeDevice();

  vk::SurfaceFormatKHR chooseSwapSurfaceFormat();

  vk::PresentModeKHR chooseSwapPresentMode();

  vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

  void initializeSwapChain();

  void initializeCommandBuffers();

  void buildSyncObjects();

  void drawFrame();

  /*
  virtual void initialize() = 0;

  virtual void draw() = 0;*/

  void mainLoop();

  virtual ~ExampleBase();

 protected:
  Window window;
  logi::VulkanInstance instance_;
  logi::SurfaceKHR surface;
  logi::PhysicalDevice physicalDevice_;
  logi::LogicalDevice logicalDevice_;
  logi::QueueFamily graphicsFamily_;
  logi::QueueFamily presentFamily_;
  logi::Queue graphicsQueue_;
  logi::Queue presentQueue_;
  logi::SwapchainKHR swapchain_;
  std::vector<logi::Image> swapchainImages_;
  vk::Extent2D swapchainImageExtent_;
  vk::Format swapchainImageFormat_;

  logi::CommandPool graphicsFamilyCmdPool_;
  std::vector<logi::CommandBuffer> primaryGraphicsCmdBuffers_;

  size_t currentFrame_ = 0u;
  std::vector<logi::Semaphore> imageAvailableSemaphores_;
  std::vector<logi::Semaphore> renderFinishedSemaphores_;
  std::vector<logi::Fence> inFlightFences_;

  ExampleConfiguration config_;
};

#endif