/**
 * Project Logi source code
 * Copyright (C) 2019 Lana Besevic
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

#ifndef LOGI_DESCRIPTOR_DESCRIPTOR_POOL_IMPL_HPP
#define LOGI_DESCRIPTOR_DESCRIPTOR_POOL_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"
#include "logi/structures/extension.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;
class DescriptorSetImpl;

class DescriptorPoolImpl : public VulkanObject<DescriptorPoolImpl>, public VulkanObjectComposite<DescriptorSetImpl> {
 public:
  DescriptorPoolImpl(LogicalDeviceImpl& logicalDevice, const vk::DescriptorPoolCreateInfo& createInfo,
                     const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  std::vector<std::shared_ptr<DescriptorSetImpl>>
    allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                           const ConstVkNextProxy<vk::DescriptorSetAllocateInfo>& next = {});

  vk::ResultValueType<void>::type freeDescriptorSets(const std::vector<size_t>& descriptorSetIds);

  vk::ResultValueType<void>::type
    reset(const vk::DescriptorPoolResetFlags& flags = vk::DescriptorPoolResetFlags()) const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator vk::DescriptorPool() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::DescriptorPool vkDescriptorPool_;
};

} // namespace logi

#endif // LOGI_DESCRIPTOR_POOL_IMPL_HPP
