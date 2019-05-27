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

#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/base/exception.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/debug_report_callback.hpp"
#include "logi/instance/debug_report_callback_impl.hpp"
#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/debug_utils_messenger_ext_impl.hpp"
#include "logi/surface/surface.hpp"
#include "logi/surface/surface_impl.hpp"

namespace logi {

VulkanInstanceImpl::VulkanInstanceImpl(const vk::InstanceCreateInfo& createInfo, PFN_vkCreateInstance pfnCreateInstance,
                                       PFN_vkGetInstanceProcAddr pfnGetProcAddr,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : allocator_(allocator) {
  // Create initial dispatch with the given function pointer.
  vk::DispatchLoaderDynamic initDispatch;
  initDispatch.vkCreateInstance = pfnCreateInstance;

  vkInstance_ = vk::createInstance(createInfo, allocator_ ? &allocator_.value() : nullptr, initDispatch);
  dispatcher_ = vk::DispatchLoaderDynamic(static_cast<VkInstance>(vkInstance_), pfnGetProcAddr);

  // Fetch available physical devices.
  std::vector<vk::PhysicalDevice> devices = vkInstance_.enumeratePhysicalDevices(dispatcher_);

  // Create Vulkan device handles.
  for (vk::PhysicalDevice& device : devices) {
    VulkanObjectComposite<PhysicalDeviceImpl>::createObject(*this, device);
  }
}

// region Sub-Handles

DebugReportCallbackEXT
  VulkanInstanceImpl::createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return DebugReportCallbackEXT(
    VulkanObjectComposite<DebugReportCallbackEXTImpl>::createObject(*this, createInfo, allocator));
}

void VulkanInstanceImpl::destroyDebugReportCallbackEXT(size_t id) {
  VulkanObjectComposite<DebugReportCallbackEXTImpl>::destroyObject(id);
}

DebugUtilsMessengerEXT
  VulkanInstanceImpl::createDebugUtilsMessengerEXT(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return DebugUtilsMessengerEXT(
    VulkanObjectComposite<DebugUtilsMessengerEXTImpl>::createObject(*this, createInfo, allocator));
}

void VulkanInstanceImpl::destroyDebugUtilsMessengerEXT(size_t id) {
  VulkanObjectComposite<DebugUtilsMessengerEXTImpl>::destroyObject(id);
}

SurfaceKHR VulkanInstanceImpl::registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                                  const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, vkSurface, allocator));
}

#ifdef VK_USE_PLATFORM_ANDROID_KHR
SurfaceKHR VulkanInstanceImpl::createAndroidSurfaceKHR(const vk::AndroidSurfaceCreateInfoKHR& createInfo,
                                                       const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

SurfaceKHR VulkanInstanceImpl::createDisplayPlaneSurfaceKHR(const vk::DisplaySurfaceCreateInfoKHR& createInfo,
                                                            const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}

#ifdef VK_USE_PLATFORM_IOS_MVK
SurfaceKHR VulkanInstanceImpl::createIOSSurfaceMVK(const vk::IOSSurfaceCreateInfoMVK& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA
SurfaceKHR VulkanInstanceImpl::createImagePipeSurfaceFUCHSIA(const vk::ImagePipeSurfaceCreateInfoFUCHSIA& createInfo,
                                                             const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK
SurfaceKHR VulkanInstanceImpl::createMacOSSurfaceMVK(const vk::MacOSSurfaceCreateInfoMVK& createInfo,
                                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT
SurfaceKHR VulkanInstanceImpl::createMetalSurfaceEXT(const vk::MetalSurfaceCreateInfoEXT& createInfo,
                                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP
SurfaceKHR
  VulkanInstanceImpl::createStreamDescriptorSurfaceGGP(const vk::StreamDescriptorSurfaceCreateInfoGGP& createInfo,
                                                       const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN
SurfaceKHR VulkanInstanceImpl::createViSurfaceNN(const vk::ViSurfaceCreateInfoNN& createInfo,
                                                 const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
SurfaceKHR VulkanInstanceImpl::createWaylandSurfaceKHR(const vk::WaylandSurfaceCreateInfoKHR& createInfo,
                                                       const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
SurfaceKHR VulkanInstanceImpl::createWin32SurfaceKHR(const vk::Win32SurfaceCreateInfoKHR& createInfo,
                                                     const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
SurfaceKHR VulkanInstanceImpl::createXcbSurfaceKHR(const vk::XcbSurfaceCreateInfoKHR& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
SurfaceKHR VulkanInstanceImpl::createXlibSurfaceKHR(const vk::XlibSurfaceCreateInfoKHR& createInfo,
                                                    const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, createInfo, allocator));
}
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

void VulkanInstanceImpl::destroySurfaceKHR(size_t id) {
  VulkanObjectComposite<SurfaceKHRImpl>::destroyObject(id);
}

// endregion

// region Vulkan Commands

std::vector<PhysicalDevice> VulkanInstanceImpl::enumeratePhysicalDevices() const {
  auto ownedDevices = VulkanObjectComposite<PhysicalDeviceImpl>::getHandles();

  std::vector<PhysicalDevice> devices;
  devices.reserve(ownedDevices.size());

  for (const auto& entry : ownedDevices) {
    devices.emplace_back(entry.second);
  }

  return devices;
}

void VulkanInstanceImpl::debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags,
                                               vk::DebugReportObjectTypeEXT objectType, uint64_t object,
                                               size_t location, int32_t messageCode, const std::string& layerPrefix,
                                               const std::string& message) const {
  vkInstance_.debugReportMessageEXT(flags, objectType, object, location, messageCode, layerPrefix, message,
                                    getDispatcher());
}

void VulkanInstanceImpl::submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    const vk::DebugUtilsMessageTypeFlagsEXT& messageTypes,
                                                    const vk::DebugUtilsMessengerCallbackDataEXT& callbackData) const {
  vkInstance_.submitDebugUtilsMessageEXT(messageSeverity, messageTypes, callbackData, getDispatcher());
}

PFN_vkVoidFunction VulkanInstanceImpl::getInstanceProcAddr(const std::string& name) const {
  return vkInstance_.getProcAddr(name, getDispatcher());
}

// endregion Vulkan Commands

const vk::DispatchLoaderDynamic& VulkanInstanceImpl::getDispatcher() const {
  return dispatcher_;
}

VulkanInstanceImpl::operator vk::Instance() const {
  return vkInstance_;
}

void VulkanInstanceImpl::free() {
  VulkanObjectComposite<PhysicalDeviceImpl>::destroyAllObjects();

  vkInstance_.destroy(allocator_ ? &allocator_.value() : nullptr);
  VulkanObject::free();
}

void VulkanInstanceImpl::destroy() {
  free();
}

} // namespace logi
