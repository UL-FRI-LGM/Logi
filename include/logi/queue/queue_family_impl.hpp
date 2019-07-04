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
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class QueueImpl;
class CommandPoolImpl;

class QueueFamilyImpl : public VulkanObject<QueueFamilyImpl>,
                        public VulkanObjectComposite<QueueImpl>,
                        public VulkanObjectComposite<CommandPoolImpl> {
 public:
  QueueFamilyImpl(LogicalDeviceImpl& logicalDevice, const vk::DeviceQueueCreateInfo& createInfo);

  const std::shared_ptr<CommandPoolImpl>&
    createCommandPool(const vk::CommandPoolCreateFlags& flags = {},
                      const ConstVkNextProxy<vk::CommandPoolCreateInfo>& next = {},
                      const std::optional<vk::AllocationCallbacks>& allocator = {});

  void destroyCommandPool(size_t id);

  // region Logi Declarations

  const std::shared_ptr<QueueImpl>& getQueue(uint32_t queueIndex);

  const std::shared_ptr<QueueImpl>& getQueue2(uint32_t queueIndex, const vk::DeviceQueueCreateFlags& flags = {},
                                              const ConstVkNextProxy<vk::DeviceQueueInfo2>& next = {});

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
  LogicalDeviceImpl& logicalDevice_;
  uint32_t queueFamilyIndex_;
  uint32_t queueCount_;
};

} // namespace logi

#endif // LOGI_QUEUE_QUEUE_FAMILY_IMPL_HPP
