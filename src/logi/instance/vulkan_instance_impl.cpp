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

namespace logi {

VulkanInstanceImpl::VulkanInstanceImpl(const vk::InstanceCreateInfo& create_info,
                                       PFN_vkCreateInstance pfn_create_instance,
                                       PFN_vkGetInstanceProcAddr pfn_get_instance_proc_addr,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : allocator_(allocator) {
  // Create initial dispatch with the given function pointer.
  vk::DispatchLoaderDynamic init_dispatch;
  init_dispatch.vkCreateInstance = pfn_create_instance;

  vk_instance_ = vk::createInstance(create_info, allocator_ ? &allocator.value() : nullptr, init_dispatch);
  dispatcher_ = vk::DispatchLoaderDynamic(vk_instance_, pfn_get_instance_proc_addr);

  // Fetch available physical devices.
  std::vector<vk::PhysicalDevice> devices = vk_instance_.enumeratePhysicalDevices(dispatcher_);

  // Create Vulkan device handles.
  for (vk::PhysicalDevice& device : devices) {
    VulkanObjectComposite<PhysicalDeviceImpl>::createObject(*this, device);
  }
}

DebugReportCallbackEXT
  VulkanInstanceImpl::createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& create_info,
                                                   const std::optional<vk::AllocationCallbacks>& allocator = {}) {
  return DebugReportCallbackEXT(
    VulkanObjectComposite<DebugReportCallbackEXTImpl>::createObject(*this, create_info, allocator));
}

void VulkanInstanceImpl::destroyDebugReportCallbackEXT(size_t id) {
  VulkanObjectComposite<DebugReportCallbackEXTImpl>::destroyObject(id);
}

std::vector<PhysicalDevice> VulkanInstanceImpl::enumeratePhysicalDevices() const {
  auto devices_map = VulkanObjectComposite<PhysicalDeviceImpl>::getHandles();

  std::vector<PhysicalDevice> devices;
  devices.reserve(devices_map.size());

  for (const auto& entry : devices_map) {
    devices.emplace_back(entry.second);
  }

  return devices;
}

const vk::DispatchLoaderDynamic& VulkanInstanceImpl::getDispatcher() const {
  return dispatcher_;
}

VulkanInstanceImpl::operator vk::Instance() const {
  return vk_instance_;
}

void VulkanInstanceImpl::free() {
  if (valid()) {
    VulkanObjectComposite<PhysicalDeviceImpl>::destroyAllObjects();

    for (const auto& callback : debug_callbacks_) {
      vk_instance_.destroyDebugReportCallbackEXT(callback, nullptr, dispatcher_);
    }

    vk_instance_.destroy(allocator_ ? &allocator_.value() : nullptr);
    VulkanObject::free();
  }
}

void VulkanInstanceImpl::destroy() {
  free();
}

} // namespace logi