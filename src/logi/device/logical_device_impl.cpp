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
#include "logi/queue/queue_family.hpp"
#include "logi/queue/queue_family_impl.hpp"

namespace logi {

LogicalDeviceImpl::LogicalDeviceImpl(PhysicalDeviceImpl& physicalDevice, const vk::DeviceCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator)
  : physicalDevice_(physicalDevice), allocator_(allocator) {
  vk::Instance vkInstance = physicalDevice.getInstance();
  vk::PhysicalDevice vkPhysicalDevice = physicalDevice;
  const vk::DispatchLoaderDynamic& instanceDispatcher = physicalDevice.getDispatcher();

  vkDevice_ = vkPhysicalDevice.createDevice(createInfo, allocator_ ? &allocator.value() : nullptr, instanceDispatcher);

  // Initialize device dispatcher.
  dispatcher_ = vk::DispatchLoaderDynamic(static_cast<VkInstance>(vkInstance), instanceDispatcher.vkGetInstanceProcAddr,
                                          static_cast<VkDevice>(vkDevice_), instanceDispatcher.vkGetDeviceProcAddr);

  // Initialize queue families.
  for (uint32_t i = 0u; i < createInfo.queueCreateInfoCount; i++) {
    VulkanObjectComposite<QueueFamilyImpl>::createObject(*this, createInfo.pQueueCreateInfos[i]);
  }
}

std::vector<QueueFamily> LogicalDeviceImpl::getQueueFamilies() const {
  auto familiesMap = VulkanObjectComposite<QueueFamilyImpl>::getHandles();

  std::vector<QueueFamily> queueFamilies;
  queueFamilies.reserve(familiesMap.size());

  for (const auto& entry : familiesMap) {
    queueFamilies.emplace_back(entry.second);
  }

  return queueFamilies;
}

VulkanInstanceImpl& LogicalDeviceImpl::getInstance() const {
  return physicalDevice_.getInstance();
}

PhysicalDeviceImpl& LogicalDeviceImpl::getPhysicalDevice() const {
  return physicalDevice_;
}

const vk::DispatchLoaderDynamic& LogicalDeviceImpl::getDispatcher() const {
  return dispatcher_;
}

void LogicalDeviceImpl::destroy() const {
  physicalDevice_.destroyLogicalDevice(id());
}

LogicalDeviceImpl::operator const vk::Device() const {
  return vkDevice_;
}

void LogicalDeviceImpl::free() {
  // TODO (Destroy composited objects)
  VulkanObjectComposite<QueueFamilyImpl>::destroyAllObjects();
  vkDevice_.destroy(allocator_ ? &allocator_.value() : nullptr, dispatcher_);
  vkDevice_ = nullptr;
  VulkanObject::free();
}

} // namespace logi
