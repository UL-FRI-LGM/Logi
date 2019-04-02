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

#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

LogicalDeviceImpl::LogicalDeviceImpl(PhysicalDeviceImpl& physical_device, const vk::DeviceCreateInfo& create_info,
                                     const std::optional<vk::AllocationCallbacks>& allocator)
  : physical_device_(physical_device), allocator_(allocator) {
  vk::Instance vk_instance = physical_device.getInstance();
  vk::PhysicalDevice vk_physical_device = physical_device;
  const vk::DispatchLoaderDynamic& instance_dispatcher = physical_device.getDispatcher();

  vk_device_ =
    vk_physical_device.createDevice(create_info, allocator_ ? &allocator.value() : nullptr, instance_dispatcher);

  // Initialize device dispatcher.
  dispatcher_ = vk::DispatchLoaderDynamic(vk_instance, instance_dispatcher.vkGetInstanceProcAddr, vk_device_,
                                          instance_dispatcher.vkGetDeviceProcAddr);
}

VulkanInstanceImpl& LogicalDeviceImpl::getInstance() const {
  return physical_device_.getInstance();
}

PhysicalDeviceImpl& LogicalDeviceImpl::getPhysicalDevice() const {
  return physical_device_;
}

const vk::DispatchLoaderDynamic& LogicalDeviceImpl::getDispatcher() const {
  return dispatcher_;
}

void LogicalDeviceImpl::destroy() const {
  physical_device_.destroyLogicalDevice(id());
}

LogicalDeviceImpl::operator const vk::Device() const {
  return vk_device_;
}

void LogicalDeviceImpl::free() {
  // TODO (Destroy composited objects)
  vk_device_.destroy(allocator_ ? &allocator_.value() : nullptr, dispatcher_);
  vk_device_ = nullptr;
  VulkanObject::free();
}

}