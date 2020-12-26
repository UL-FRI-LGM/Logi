#include "example_base.h"
#include <example_base.h>

PipelineLayoutData::PipelineLayoutData(logi::PipelineLayout layout,
                                       std::vector<logi::DescriptorSetLayout> descriptorSetLayouts)
  : layout(std::move(layout)), descriptorSetLayouts(std::move(descriptorSetLayouts)) {}

ExampleBase::ExampleBase(const ExampleConfiguration& config) : config_(config), vulkanState_() {}

void ExampleBase::run() {
  initWindow();
  initInput();
  createInstance();
  initSurface();
  selectDevice();
  initializeDevice();
  initializeSwapChain();
  buildSyncObjects();
  initializeCommandBuffers();
  initialize();
  mainLoop();
}

VkBool32 ExampleBase::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj_type, uint64_t obj,
                                    size_t location, int32_t code, const char* layer_prefix, const char* msg,
                                    void* user_data) {
  std::cout << "Validation layer: " << msg << std::endl;
  return VK_FALSE;
}

void ExampleBase::initWindow() {
  window_ = cppglfw::GLFWManager::instance().createWindow(config_.windowTitle, config_.windowWidth,
                                                          config_.windowHeight, {{GLFW_CLIENT_API, GLFW_NO_API}});
}

void ExampleBase::initInput() {
  window_.setOnScrollCallback("onScroll", [this](const cppglfw::Window&, double, double yOffset) {
    zoom += (float) (0.1 * yOffset);
    viewChanged = true;
  });

  window_.setOnMouseButtonCallback("onClick", [this](const cppglfw::Window&, int32_t button, int32_t action, int32_t) {
    switch (button) {
      case GLFW_MOUSE_BUTTON_LEFT:
        mouseButtons.left = action;
        break;
      case GLFW_MOUSE_BUTTON_RIGHT:
        mouseButtons.right = action;
        break;
      case GLFW_MOUSE_BUTTON_MIDDLE:
        mouseButtons.middle = action;
        break;
      default:
        return;
    }
  });

  window_.setOnCursorMoveCallback("onCursorMove", [this](const cppglfw::Window&, double xPos, double yPos) {
    glm::vec2 newPos = glm::vec2(xPos, yPos);
    glm::vec2 dPos = newPos - mousePos;

    // Rotate object
    if (mouseButtons.right) {
      rotation.y += dPos.x * 0.2f;
      rotation.x -= dPos.y * 0.25f;
      rotation.x = std::max(-75.0f, std::min(75.0f, rotation.x));
      viewChanged = true;
    }

    // Move camera
    if (mouseButtons.left) {
      cameraPos.x += dPos.x * 0.01f;
      cameraPos.y -= dPos.y * 0.01f;
      viewChanged = true;
    }

    mousePos = newPos;
  });
}

void ExampleBase::createInstance() {
  // Add required extensions.
  std::vector<const char*> extensions;
  extensions = cppglfw::GLFWManager::instance().getRequiredInstanceExtensions();
  extensions.insert(extensions.end(), config_.instanceExtensions.begin(), config_.instanceExtensions.end());
  extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

  // Remove duplicate extension.
  std::sort(extensions.begin(), extensions.end());
  extensions.erase(std::unique(extensions.begin(), extensions.end()), extensions.end());

  // Create instance.
  vk::InstanceCreateInfo instanceCI;
  instanceCI.ppEnabledLayerNames = config_.validationLayers.data();
  instanceCI.enabledLayerCount = static_cast<uint32_t>(config_.validationLayers.size());
  instanceCI.ppEnabledExtensionNames = extensions.data();
  instanceCI.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

  vulkanState_.setInstance(logi::createInstance(
    instanceCI, reinterpret_cast<PFN_vkCreateInstance>(glfwGetInstanceProcAddress(nullptr, "vkCreateInstance")),
    reinterpret_cast<PFN_vkGetInstanceProcAddr>(glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr"))));

  // Setup debug report callback.
  vk::DebugReportCallbackCreateInfoEXT debugReportCI;
  debugReportCI.flags =
    vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning;
  debugReportCI.pfnCallback = debugCallback;

  vulkanState_.instance_.createDebugReportCallbackEXT(debugReportCI);
}

void ExampleBase::initSurface() {
  surface_ = vulkanState_.instance_.registerSurfaceKHR(window_.createWindowSurface(vulkanState_.instance_).value);
}

// TODO: account for requested device extensions
void ExampleBase::selectDevice() {
  // Select GPU
  const std::vector<logi::PhysicalDevice>& devices = vulkanState_.instance_.enumeratePhysicalDevices();

  for (const auto& device : devices) {
    vk::PhysicalDeviceType type = device.getProperties().deviceType;

    if (type == vk::PhysicalDeviceType::eDiscreteGpu) {
      // If discrete gpu is found select it immediately.
      vulkanState_.physicalDevice_ = device;
      return;
    } else if (type == vk::PhysicalDeviceType::eIntegratedGpu || type == vk::PhysicalDeviceType::eVirtualGpu) {
      vulkanState_.physicalDevice_ = device;
    }
  }

  // Assert if no device is found.
  assert(vulkanState_.physicalDevice_);
}

void ExampleBase::initializeDevice() {
  std::vector<vk::QueueFamilyProperties> familyProperties = vulkanState_.physicalDevice_.getQueueFamilyProperties();

  // Search for graphical queue family.
  uint32_t graphicsFamilyIdx = std::numeric_limits<uint32_t>::max();
  uint32_t presentFamilyIdx = std::numeric_limits<uint32_t>::max();

  for (uint32_t i = 0; i < familyProperties.size(); i++) {
    if (familyProperties[i].queueFlags | vk::QueueFlagBits::eGraphics) {
      graphicsFamilyIdx = i;
    }

    // Check if queue family supports present.
    if (vulkanState_.physicalDevice_.getSurfaceSupportKHR(graphicsFamilyIdx, surface_)) {
      presentFamilyIdx = graphicsFamilyIdx;
    }

    // Stop once both graphical and present queue families are found.
    if (graphicsFamilyIdx != std::numeric_limits<uint32_t>::max() &&
        presentFamilyIdx != std::numeric_limits<uint32_t>::max()) {
      break;
    }
  }

  if (graphicsFamilyIdx == std::numeric_limits<uint32_t>::max()) {
    throw std::runtime_error("Failed to find graphical queue.");
  }
  if (presentFamilyIdx == std::numeric_limits<uint32_t>::max()) {
    throw std::runtime_error("Failed to find queue family that supports presentation.");
  }

  std::vector<const char*> extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  extensions.insert(extensions.end(), config_.deviceExtensions.begin(), config_.deviceExtensions.end());

  static const std::array<float, 1> kPriorities = {1.0f};

  std::vector<vk::DeviceQueueCreateInfo> queueCIs;
  queueCIs.emplace_back(vk::DeviceQueueCreateFlags(), graphicsFamilyIdx, 1u, kPriorities.data());
  if (graphicsFamilyIdx != presentFamilyIdx) {
    queueCIs.emplace_back(vk::DeviceQueueCreateFlags(), presentFamilyIdx, 1u, kPriorities.data());
  }

  vk::DeviceCreateInfo deviceCI;
  deviceCI.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  deviceCI.ppEnabledExtensionNames = extensions.data();
  deviceCI.queueCreateInfoCount = static_cast<uint32_t>(queueCIs.size());
  deviceCI.pQueueCreateInfos = queueCIs.data();
  
  vulkanState_.addLogicalDevice("MainLogical", vulkanState_.physicalDevice_.createLogicalDevice(deviceCI));
  vulkanState_.setDefaultLogicalDevice("MainLogical");

  std::vector<logi::QueueFamily> queueFamilies = vulkanState_.defaultLogicalDevice_->enumerateQueueFamilies();
  for (const auto& family : queueFamilies) {
    if (static_cast<uint32_t>(family) == graphicsFamilyIdx) {
      vulkanState_.graphicsFamily_ = family;
    }
    if (static_cast<uint32_t>(family) == presentFamilyIdx) {
      vulkanState_.presentFamily_ = family;
    }
  }

  assert(vulkanState_.graphicsFamily_);
  assert(vulkanState_.presentFamily_);

  vulkanState_.addQueue("GraphicsQueue", vulkanState_.graphicsFamily_.getQueue(0));
  vulkanState_.addQueue("PresentQueue", vulkanState_.presentFamily_.getQueue(0));
  vulkanState_.setDefaultGraphicsQueue("GraphicsQueue");
  vulkanState_.setDefaultPresentQueue("PresentQueue");
}

ExampleBase::~ExampleBase() {
  if (vulkanState_.instance_) {
    vulkanState_.instance_.destroy();
  }
}

vk::SurfaceFormatKHR ExampleBase::chooseSwapSurfaceFormat() {
  const std::vector<vk::SurfaceFormatKHR>& availableFormats = vulkanState_.physicalDevice_.getSurfaceFormatsKHR(surface_);

  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR ExampleBase::chooseSwapPresentMode() {
  const std::vector<vk::PresentModeKHR>& availablePresentModes = vulkanState_.physicalDevice_.getSurfacePresentModesKHR(surface_);

  vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
      return availablePresentMode;
    } else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
      bestMode = availablePresentMode;
    }
  }

  return bestMode;
}

vk::Extent2D ExampleBase::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    vk::Extent2D actualExtent(window_.getSize().first, window_.getSize().second);

    actualExtent.width =
      std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height =
      std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

void ExampleBase::initializeSwapChain() {
  vk::SurfaceCapabilitiesKHR capabilities = vulkanState_.physicalDevice_.getSurfaceCapabilitiesKHR(surface_);

  vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
  vk::PresentModeKHR presentMode = chooseSwapPresentMode();
  vk::Extent2D extent = chooseSwapExtent(capabilities);

  imageCount_ = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && imageCount_ > capabilities.maxImageCount) {
    imageCount_ = capabilities.maxImageCount;
  }

  logi::SwapchainKHR oldSwapchain = swapchain_;

  vk::SwapchainCreateInfoKHR createInfo = {};
  createInfo.surface = surface_;
  createInfo.minImageCount = imageCount_;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  if (vulkanState_.graphicsFamily_ != vulkanState_.presentFamily_) {
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.queueFamilyIndexCount = 2;
    uint32_t indices[2]{vulkanState_.graphicsFamily_, vulkanState_.presentFamily_};
    createInfo.pQueueFamilyIndices = indices;
  } else {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  }

  createInfo.preTransform = capabilities.currentTransform;
  createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = static_cast<vk::SwapchainKHR>(oldSwapchain);

  swapchain_ = vulkanState_.defaultLogicalDevice_->createSwapchainKHR(createInfo);
  // Destroy old swapchain and clear the images.
  if (oldSwapchain) {
    oldSwapchain.destroy();
  }
  swapchainImages_.clear();
  swapchainImageViews_.clear();

  swapchainImages_ = swapchain_.getImagesKHR();
  swapchainImageFormat_ = surfaceFormat.format;
  swapchainImageExtent_ = extent;

  // Create image views
  swapchainImageViews_.reserve(swapchainImages_.size());

  for (auto& image : swapchainImages_) {
    swapchainImageViews_.emplace_back(image.createImageView(
      vk::ImageViewCreateFlags(), vk::ImageViewType::e2D, swapchainImageFormat_, vk::ComponentMapping(),
      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));
  }
}

void ExampleBase::initializeCommandBuffers() {
  vulkanState_.addCommandPool("GraphicsFamilyCmd", vulkanState_.graphicsFamily_.createCommandPool(vk::CommandPoolCreateFlagBits::eResetCommandBuffer));
  vulkanState_.setDefaultGraphicsCommandPool("GraphicsFamilyCmd");

  primaryGraphicsCmdBuffers_ =
    vulkanState_.defaultGraphicsCommandPool_->allocateCommandBuffers(vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(swapchainImages_.size()));
}

void ExampleBase::buildSyncObjects() {
  for (size_t i = 0; i < config_.maxFramesInFlight; i++) {
    imageAvailableSemaphores_.emplace_back(vulkanState_.defaultLogicalDevice_->createSemaphore(vk::SemaphoreCreateInfo()));
    renderFinishedSemaphores_.emplace_back(vulkanState_.defaultLogicalDevice_->createSemaphore(vk::SemaphoreCreateInfo()));
    inFlightFences_.emplace_back(vulkanState_.defaultLogicalDevice_->createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)));
  }
}

void ExampleBase::onViewChanged() {}

void ExampleBase::imGUI_createUI() {} // Prevent necessity to implement in subclass

logi::CommandBuffer* ExampleBase::imGUI_createOverlay(const uint32_t& i) {
  return nullptr;
}

void ExampleBase::recreateSwapChain() {
  vulkanState_.defaultLogicalDevice_->waitIdle();

  initializeSwapChain();
  onSwapChainRecreate();
}

void ExampleBase::drawFrame() {
  try {
    // Wait if drawing is still in progress.
    inFlightFences_[currentFrame_].wait(std::numeric_limits<uint64_t>::max());

    // Acquire next image.
    const uint32_t imageIndex =
      swapchain_
        .acquireNextImageKHR(std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores_[currentFrame_], nullptr)
        .value;
    inFlightFences_[currentFrame_].reset();

    static const vk::PipelineStageFlags wait_stages{vk::PipelineStageFlagBits::eColorAttachmentOutput};


    draw(); // Example draw.
    logi::CommandBuffer* overlayCommandBuffer = imGUI_createOverlay(imageIndex); // Overlay command buffer - ImGUI
    
    // Command buffers to submit
    std::vector<vk::CommandBuffer> submitCommandBuffers = {primaryGraphicsCmdBuffers_[imageIndex]};
    if(overlayCommandBuffer) {
      submitCommandBuffers.emplace_back(*overlayCommandBuffer);
    }

    vk::SubmitInfo submit_info;
    submit_info.pWaitDstStageMask = &wait_stages;
    submit_info.pWaitSemaphores = &static_cast<const vk::Semaphore&>(imageAvailableSemaphores_[currentFrame_]);
    submit_info.waitSemaphoreCount = 1u;

    submit_info.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
    submit_info.pCommandBuffers = submitCommandBuffers.data();

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &static_cast<const vk::Semaphore&>(renderFinishedSemaphores_[currentFrame_]);

    vulkanState_.defaultGraphicsQueue_->submit({submit_info}, inFlightFences_[currentFrame_]);

    // Present image.
    vulkanState_.defaultPresentQueue_->presentKHR(
      vk::PresentInfoKHR(1, &static_cast<const vk::Semaphore&>(renderFinishedSemaphores_[currentFrame_]), 1,
                         &static_cast<const vk::SwapchainKHR&>(swapchain_), &imageIndex));

    currentFrame_ = (currentFrame_ + 1) % config_.maxFramesInFlight;
  } catch (const vk::OutOfDateKHRError&) {
    recreateSwapChain();
  }

  // Notify if view changed.
  if (viewChanged) {
    onViewChanged();
    viewChanged = false;
  }
}

void ExampleBase::mainLoop() {
  while (!window_.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }

  vulkanState_.defaultLogicalDevice_->waitIdle();
}

