#ifndef BASE_EXAMPLE_BASE_H
#define BASE_EXAMPLE_BASE_H

#define GLFW_INCLUDE_VULKAN
#include <cppglfw/CppGLFW.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include "logi/logi.hpp"
#include "utility.h"
#include "vulkanState.h"

struct ExampleConfiguration {
  std::string windowTitle = "Example";
  int32_t windowWidth = 1280;
  int32_t windowHeight = 720;
  size_t maxFramesInFlight = 2u;
  std::vector<const char*> instanceExtensions;
  std::vector<const char*> deviceExtensions;
  std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
};

struct PipelineLayoutData {
  explicit PipelineLayoutData(logi::PipelineLayout layout = {},
                              std::vector<logi::DescriptorSetLayout> descriptorSetLayouts = {});
  ;

  logi::PipelineLayout layout;
  std::vector<logi::DescriptorSetLayout> descriptorSetLayouts;
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

  void initInput();

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

  virtual void recreateSwapChain(); // Only redefined by imGUI_base

  virtual void initialize() = 0;

  virtual void onViewChanged();

  virtual void onSwapChainRecreate() = 0;

  virtual logi::CommandBuffer* imGUI_createOverlay(const uint32_t& i); // Only redefined by imGUI_base

  virtual void imGUI_createUI(); // Used by imGUI applications

  virtual void draw() = 0;

  virtual void mainLoop(); // Only redefined by imGUI_base

  virtual ~ExampleBase();

 protected:
  VulkanState vulkanState_;

  cppglfw::Window window_;
  logi::SurfaceKHR surface_;

  // Swapchain data
  uint32_t imageCount_;
  logi::SwapchainKHR swapchain_;
  std::vector<logi::SwapchainImage> swapchainImages_;
  std::vector<logi::ImageView> swapchainImageViews_;
  vk::Extent2D swapchainImageExtent_;
  vk::Format swapchainImageFormat_;
  
  std::vector<logi::CommandBuffer> primaryGraphicsCmdBuffers_;

  size_t currentFrame_ = 0u;
  std::vector<logi::Semaphore> imageAvailableSemaphores_;
  std::vector<logi::Semaphore> renderFinishedSemaphores_;
  std::vector<logi::Fence> inFlightFences_;

  ExampleConfiguration config_;

  struct {
    bool left = false;
    bool right = false;
    bool middle = false;
  } mouseButtons;
  glm::vec2 mousePos;

  glm::vec3 rotation = glm::vec3();
  glm::vec3 cameraPos = glm::vec3();
  float zoom = 0;

  bool viewChanged = false;
};

#endif