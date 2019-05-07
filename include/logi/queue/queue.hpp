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

#ifndef LOGI_QUEUE_QUEUE_HPP
#define LOGI_QUEUE_QUEUE_HPP

#include "logi/base/handle.hpp"
#include "logi/queue/queue_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class QueueFamily;

class Queue : public Handle<QueueImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  vk::ResultValueType<void>::type submit(const std::vector<vk::SubmitInfo>& submits, vk::Fence fence = {}) const;

  vk::ResultValueType<void>::type bindSparse(const std::vector<vk::BindSparseInfo>& bind_info,
                                             vk::Fence fence = {}) const;

  vk::ResultValueType<void>::type waitIdle() const;

  vk::Result presentKHR(const vk::PresentInfoKHR& present_info) const;

  void beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const;

  void insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const;

  void endDebugUtilsLabelEXT() const;

  std::vector<vk::CheckpointDataNV> getCheckpointDataNV() const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  QueueFamily getQueueFamily() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator vk::Queue() const;

  void destroy();

  // endregion
};

} // namespace logi

#endif // LOGI_QUEUE_QUEUE_HPP
