/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/debug_report_callback.hpp"
#include "logi/instance/debug_report_callback_impl.hpp"
#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/debug_utils_messenger_ext_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/surface/surface_khr.hpp"
#include "logi/surface/surface_khr_impl.hpp"

namespace logi {

DebugReportCallbackEXT
  VulkanInstance::createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return DebugReportCallbackEXT(object_->createDebugReportCallbackEXT(createInfo, allocator));
}

void VulkanInstance::destroyDebugReportCallbackEXT(const DebugReportCallbackEXT& callback) const {
  object_->destroyDebugReportCallbackEXT(callback.id());
}

DebugUtilsMessengerEXT
  VulkanInstance::createDebugUtilsMessengerEXT(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return DebugUtilsMessengerEXT(object_->createDebugUtilsMessengerEXT(createInfo, allocator));
}

void VulkanInstance::destroyDebugUtilsMessengerEXT(const DebugUtilsMessengerEXT& messenger) const {
  object_->destroyDebugUtilsMessengerEXT(messenger.id());
}

SurfaceKHR VulkanInstance::registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                              const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->registerSurfaceKHR(vkSurface, allocator));
}

#ifdef VK_USE_PLATFORM_ANDROID_KHR
SurfaceKHR VulkanInstance::createAndroidSurfaceKHR(const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createAndroidSurfaceKHR(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

SurfaceKHR VulkanInstance::createDisplayPlaneSurfaceKHR(const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                                                        const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createDisplayPlaneSurfaceKHR(createInfo, allocator));
}

#ifdef VK_USE_PLATFORM_IOS_MVK
SurfaceKHR VulkanInstance::createIOSSurfaceMVK(const vk::IOSSurfaceCreateInfoMVK& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createIOSSurfaceMVK(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
SurfaceKHR
  VulkanInstance::createImagePipeSurfaceFUCHSIA(const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createImagePipeSurfaceFUCHSIA(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
SurfaceKHR VulkanInstance::createMacOSSurfaceMVK(const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createMacOSSurfaceMVK(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
SurfaceKHR VulkanInstance::createMetalSurfaceEXT(const vk::MetalSurfaceCreateInfoEXT& createInfo,
                                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createMetalSurfaceEXT(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP
SurfaceKHR
  VulkanInstance::createStreamDescriptorSurfaceGGP(const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createStreamDescriptorSurfaceGGP(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
SurfaceKHR VulkanInstance::createViSurfaceNN(const vk::ViSurfaceCreateInfoNN& createInfo,
                                             const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createViSurfaceNN(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
SurfaceKHR VulkanInstance::createWaylandSurfaceKHR(const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createWaylandSurfaceKHR(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
SurfaceKHR VulkanInstance::createWin32SurfaceKHR(const vk::Win32SurfaceCreateInfoKHR& createInfo,
                                                 const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createWin32SurfaceKHR(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
SurfaceKHR VulkanInstance::createXcbSurfaceKHR(const vk::XcbSurfaceCreateInfoKHR& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createXcbSurfaceKHR(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
SurfaceKHR VulkanInstance::createXlibSurfaceKHR(const vk::XlibSurfaceCreateInfoKHR& createInfo,
                                                const std::optional<vk::AllocationCallbacks>& allocator) const {
  return SurfaceKHR(object_->createXlibSurfaceKHR(createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

void VulkanInstance::destroySurfaceKHR(const SurfaceKHR& surface) const {
  object_->destroySurfaceKHR(surface.id());
}

std::vector<PhysicalDevice> VulkanInstance::enumeratePhysicalDevices() const {
  const std::vector<std::shared_ptr<PhysicalDeviceImpl>> devices = object_->enumeratePhysicalDevices();
  return std::vector<PhysicalDevice>(devices.begin(), devices.end());
}

void VulkanInstance::debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags,
                                           vk::DebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                           int32_t messageCode, const std::string& layerPrefix,
                                           const std::string& message) const {
  object_->debugReportMessageEXT(flags, objectType, object, location, messageCode, layerPrefix, message);
}

void VulkanInstance::submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                const vk::DebugUtilsMessageTypeFlagsEXT& messageTypes,
                                                const vk::DebugUtilsMessengerCallbackDataEXT& callbackData) const {
  object_->submitDebugUtilsMessageEXT(messageSeverity, messageTypes, callbackData);
}

PFN_vkVoidFunction VulkanInstance::getProcAddr(const std::string& name) const {
  return object_->getProcAddr(name);
}

const vk::DispatchLoaderDynamic& VulkanInstance::getDispatcher() const {
  return object_->getDispatcher();
}

VulkanInstance::operator vk::Instance() const {
  return object_->operator vk::Instance();
}

void VulkanInstance::destroy() const {
  object_->destroy();
}

VulkanInstance createInstance(const vk::InstanceCreateInfo& createInfo, PFN_vkCreateInstance pfnCreateInstance,
                              PFN_vkGetInstanceProcAddr pfnGetProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanInstance(std::make_shared<VulkanInstanceImpl>(createInfo, pfnCreateInstance, pfnGetProcAddr, allocator));
}

bool checkValidationLayerSupport(const std::vector<const char*>& layers) {
  // Fetch validation layers
  std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

  // Check if all requested validation layers are present among the available layers.
  for (const char* layer : layers) {
    bool found = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(layer, layerProperties.layerName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      return false;
    }
  }

  return true;
}

} // namespace logi
