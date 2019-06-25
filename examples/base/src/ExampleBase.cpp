#include "base/ExampleBase.h"

ExampleBase::ExampleBase(const ExampleConfiguration& config) : config_(config) {}

void ExampleBase::run() {
  initWindow();
  createInstance();
  initSurface();
  selectDevice();
  initializeDevice();
  initializeSwapChain();
}

VkBool32 ExampleBase::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj_type, uint64_t obj,
                                    size_t location, int32_t code, const char* layer_prefix, const char* msg,
                                    void* user_data) {
  std::cout << "Validation layer: " << msg << std::endl;
  return VK_FALSE;
}

void ExampleBase::initWindow() {
  window = GLFWManager::instance().createWindow(config_.windowTitle, config_.windowWidth, config_.windowHeight);
}

void ExampleBase::createInstance() {
  // Add required extensions.
  std::vector<const char*> extensions;
  extensions = GLFWManager::instance().getRequiredInstanceExtensions();
  extensions.insert(extensions.end(), config_.instanceExtensions.begin(), config_.instanceExtensions.end());
  extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

  // Remove duplicate extension.
  std::sort(extensions.begin(), extensions.end());
  extensions.erase(std::unique(extensions.begin(), extensions.end()), extensions.end());

  // Create instance.
  vk::InstanceCreateInfo instanceCI;
  instanceCI.ppEnabledLayerNames = config_.validationLayers.data();
  instanceCI.enabledLayerCount = config_.validationLayers.size();
  instanceCI.ppEnabledExtensionNames = extensions.data();
  instanceCI.enabledExtensionCount = extensions.size();

  instance_ = logi::createInstance(
    instanceCI, reinterpret_cast<PFN_vkCreateInstance>(glfwGetInstanceProcAddress(nullptr, "vkCreateInstance")),
    reinterpret_cast<PFN_vkGetInstanceProcAddr>(glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr")));

  // Setup debug report callback.
  vk::DebugReportCallbackCreateInfoEXT debugReportCI;
  debugReportCI.flags =
    vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning;
  debugReportCI.pfnCallback = debugCallback;

  instance_.createDebugReportCallbackEXT(debugReportCI);
}

void ExampleBase::initSurface() {
  surface = instance_.registerSurfaceKHR(window.createWindowSurface(instance_).value);
}

void ExampleBase::selectDevice() {
  // Select GPU
  const std::vector<logi::PhysicalDevice>& devices = instance_.enumeratePhysicalDevices();

  for (const auto& device : devices) {
    vk::PhysicalDeviceType type = device.getProperties().deviceType;

    if (type == vk::PhysicalDeviceType::eDiscreteGpu) {
      // If discrete gpu is found select it immediately.
      physicalDevice_ = device;
      return;
    } else if (type == vk::PhysicalDeviceType::eIntegratedGpu || type == vk::PhysicalDeviceType::eVirtualGpu) {
      physicalDevice_ = device;
    }
  }

  // Assert if no device is found.
  assert(physicalDevice_);
}

void ExampleBase::initializeDevice() {
  std::vector<vk::QueueFamilyProperties> familyProperties = physicalDevice_.getQueueFamilyProperties();

  // Search for graphical queue family.
  uint32_t graphicalFamilyIdx = std::numeric_limits<uint32_t>::max();
  uint32_t presentFamilyIdx = std::numeric_limits<uint32_t>::max();

  for (uint32_t i = 0; i < familyProperties.size(); i++) {
    if (familyProperties[i].queueFlags | vk::QueueFlagBits::eGraphics) {
      graphicalFamilyIdx = i;
    }

    // Check if queue family supports present.
    if (physicalDevice_.getSurfaceSupportKHR(graphicalFamilyIdx, surface)) {
      presentFamilyIdx = graphicalFamilyIdx;
    }

    // Stop once both graphical and present queue families are found.
    if (graphicalFamilyIdx != std::numeric_limits<uint32_t>::max() &&
        presentFamilyIdx != std::numeric_limits<uint32_t>::max()) {
      break;
    }
  }

  if (graphicalFamilyIdx == std::numeric_limits<uint32_t>::max()) {
    throw std::runtime_error("Failed to find graphical queue.");
  }
  if (presentFamilyIdx == std::numeric_limits<uint32_t>::max()) {
    throw std::runtime_error("Failed to find queue family that supports presentation.");
  }

  std::vector<const char*> extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  extensions.insert(extensions.end(), config_.deviceExtensions.begin(), config_.deviceExtensions.end());

  static const std::array<float, 1> kPriorities = {1.0f};

  std::vector<vk::DeviceQueueCreateInfo> queueCIs;
  queueCIs.emplace_back(vk::DeviceQueueCreateFlags(), graphicalFamilyIdx, 1u, kPriorities.data());
  if (graphicalFamilyIdx != presentFamilyIdx) {
    queueCIs.emplace_back(vk::DeviceQueueCreateFlags(), presentFamilyIdx, 1u, kPriorities.data());
  }

  vk::DeviceCreateInfo deviceCI;
  deviceCI.enabledExtensionCount = extensions.size();
  deviceCI.ppEnabledExtensionNames = extensions.data();
  deviceCI.queueCreateInfoCount = queueCIs.size();
  deviceCI.pQueueCreateInfos = queueCIs.data();

  logicalDevice_ = physicalDevice_.createLogicalDevice(deviceCI);
  std::vector<logi::QueueFamily> queueFamilies = logicalDevice_.enumerateQueueFamilies();
  for (const auto& family : queueFamilies) {
    if (static_cast<uint32_t>(family) == graphicalFamilyIdx) {
      graphicalFamily_ = family;
    }
    if (static_cast<uint32_t>(family) == presentFamilyIdx) {
      presentFamily_ = family;
    }
  }

  assert(graphicalFamily_);
  assert(presentFamily_);

  graphicalQueue_ = graphicalFamily_.getQueue(0);
  presentQueue_ = presentFamily_.getQueue(0);
}

ExampleBase::~ExampleBase() {
  if (instance_) {
    instance_.destroy();
  }
}

vk::SurfaceFormatKHR ExampleBase::chooseSwapSurfaceFormat() {
  const std::vector<vk::SurfaceFormatKHR>& availableFormats = physicalDevice_.getSurfaceFormatsKHR(surface);

  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR ExampleBase::chooseSwapPresentMode() {
  const std::vector<vk::PresentModeKHR>& availablePresentModes = physicalDevice_.getSurfacePresentModesKHR(surface);

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
    vk::Extent2D actualExtent(window.getSize().first, window.getSize().second);

    actualExtent.width =
      std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height =
      std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

void ExampleBase::initializeSwapChain() {
  vk::SurfaceCapabilitiesKHR capabilities = physicalDevice_.getSurfaceCapabilitiesKHR(surface);

  vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
  vk::PresentModeKHR presentMode = chooseSwapPresentMode();
  vk::Extent2D extent = chooseSwapExtent(capabilities);

  uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR createInfo = {};
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  if (graphicalFamily_ != presentFamily_) {
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.queueFamilyIndexCount = 2;
    uint32_t indices[2]{graphicalFamily_, presentFamily_};
    createInfo.pQueueFamilyIndices = indices;
  } else {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  }

  createInfo.preTransform = capabilities.currentTransform;
  createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = nullptr;

  swapchain_ = logicalDevice_.createSwapchainKHR(createInfo);
  swapchainImages_ = swapchain_.getImagesKHR();
  swapchainImageFormat_ = surfaceFormat.format;
  swapchainImageExtent_ = extent;
}
