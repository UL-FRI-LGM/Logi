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

DebugReportCallbackEXT
  VulkanInstanceImpl::createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                                   const std::optional<vk::AllocationCallbacks>& allocator) {
  return DebugReportCallbackEXT(
    VulkanObjectComposite<DebugReportCallbackEXTImpl>::createObject(*this, createInfo, allocator));
}

void VulkanInstanceImpl::debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags,
                                               vk::DebugReportObjectTypeEXT objectType, uint64_t object,
                                               size_t location, int32_t messageCode, const char* layerPrefix,
                                               const char* message) const {
  vkInstance_.debugReportMessageEXT(flags, objectType, object, location, messageCode, layerPrefix, message,
                                    getDispatcher());
}

void VulkanInstanceImpl::destroyDebugReportCallbackEXT(size_t id) {
  VulkanObjectComposite<DebugReportCallbackEXTImpl>::destroyObject(id);
}

SurfaceKHR VulkanInstanceImpl::registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                                  const std::optional<vk::AllocationCallbacks>& allocator) {
  return SurfaceKHR(VulkanObjectComposite<SurfaceKHRImpl>::createObject(*this, vkSurface, allocator));
}

void VulkanInstanceImpl::destroySurfaceKHR(size_t id) {
  VulkanObjectComposite<SurfaceKHRImpl>::destroyObject(id);
}

std::vector<PhysicalDevice> VulkanInstanceImpl::enumeratePhysicalDevices() const {
  auto ownedDevices = VulkanObjectComposite<PhysicalDeviceImpl>::getHandles();

  std::vector<PhysicalDevice> devices;
  devices.reserve(ownedDevices.size());

  for (const auto& entry : ownedDevices) {
    devices.emplace_back(entry.second);
  }

  return devices;
}

const vk::DispatchLoaderDynamic& VulkanInstanceImpl::getDispatcher() const {
  return dispatcher_;
}

VulkanInstanceImpl::operator vk::Instance() const {
  return vkInstance_;
}

void VulkanInstanceImpl::free() {
  VulkanObjectComposite<PhysicalDeviceImpl>::destroyAllObjects();

  for (const auto& callback : debugCallbacks_) {
    vkInstance_.destroyDebugReportCallbackEXT(callback, nullptr, dispatcher_);
  }

  vkInstance_.destroy(allocator_ ? &allocator_.value() : nullptr);
  VulkanObject::free();
}

void VulkanInstanceImpl::destroy() {
  free();
}

} // namespace logi
