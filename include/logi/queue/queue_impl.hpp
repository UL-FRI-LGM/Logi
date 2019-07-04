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

#ifndef LOGI_QUEUE_QUEUE_IMPL_HPP
#define LOGI_QUEUE_QUEUE_IMPL_HPP

#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class QueueFamilyImpl;

class QueueImpl : public VulkanObject<QueueImpl> {
 public:
  QueueImpl(QueueFamilyImpl& queueFamily, vk::Queue vkQueue);

  // region Vulkan Declarations

  vk::ResultValueType<void>::type submit(const std::vector<vk::SubmitInfo>& submits, vk::Fence fence = {}) const;

  vk::ResultValueType<void>::type bindSparse(const std::vector<vk::BindSparseInfo>& bindInfo,
                                             vk::Fence fence = {}) const;

  vk::ResultValueType<void>::type waitIdle() const;

  vk::Result presentKHR(const vk::PresentInfoKHR& presentInfo) const;

  void beginDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const;

  void insertDebugUtilsLabelEXT(const vk::DebugUtilsLabelEXT& label) const;

  void endDebugUtilsLabelEXT() const;

  std::vector<vk::CheckpointDataNV> getCheckpointDataNV() const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  QueueFamilyImpl& getQueueFamily() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::Queue&() const;

  void destroy() const;

 protected:
  void free() override;

  // endregion

 private:
  QueueFamilyImpl& queueFamily_;
  vk::Queue vkQueue_;
};

} // namespace logi

#endif // LOGI_QUEUE_QUEUE_IMPL_HPP