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

#ifndef LOGI_DEVICE_LOGICAL_DEVICE_IMPL_HPP
#define LOGI_DEVICE_LOGICAL_DEVICE_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class QueueFamilyImpl;
class QueueFamily;

class LogicalDeviceImpl : public VulkanObject<LogicalDeviceImpl>, public VulkanObjectComposite<QueueFamilyImpl> {
 public:
  LogicalDeviceImpl(PhysicalDeviceImpl& physicalDevice, const vk::DeviceCreateInfo& createInfo,
                    const std::optional<vk::AllocationCallbacks>& allocator = {});

  std::vector<QueueFamily> getQueueFamilies() const;

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Device() const;

 protected:
  void free() override;

 private:
  PhysicalDeviceImpl& physicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::Device vkDevice_;
  vk::DispatchLoaderDynamic dispatcher_;
};

} // namespace logi

#endif // LOGI_DEVICE_LOGICAL_DEVICE_IMPL_HPP
