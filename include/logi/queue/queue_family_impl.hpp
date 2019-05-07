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

#ifndef LOGI_QUEUE_QUEUE_FAMILY_IMPL_HPP
#define LOGI_QUEUE_QUEUE_FAMILY_IMPL_HPP

#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class QueueImpl;
class Queue;

class QueueFamilyImpl : public VulkanObject<QueueFamilyImpl>, public VulkanObjectComposite<QueueImpl> {
 public:
  QueueFamilyImpl(LogicalDeviceImpl& logical_device, const vk::DeviceQueueCreateInfo& queue_create_info);

  // region Logi Declarations

  Queue getQueue(uint32_t queue_index);

  Queue getQueue2(uint32_t queue_index, const vk::DeviceQueueCreateFlags& flags = {});

  void destroyQueue(size_t id);

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  uint32_t queueCount() const;

  uint32_t getIndex() const;

  operator uint32_t() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logical_device_;
  uint32_t queue_family_index_;
  uint32_t queue_count_;
};

} // namespace logi

#endif // LOGI_QUEUE_QUEUE_FAMILY_IMPL_HPP
