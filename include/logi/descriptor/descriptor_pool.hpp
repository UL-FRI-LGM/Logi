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

#ifndef LOGI_DESCRIPTOR_DESCRIPTOR_POOL_HPP
#define LOGI_DESCRIPTOR_DESCRIPTOR_POOL_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/descriptor/descriptor_pool_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;
class DescriptorSet;

class DescriptorPool : public Handle<DescriptorPoolImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkAllocateDescriptorSets.html">vkAllocateDescriptorSets</a>
   */
  std::vector<DescriptorSet>
    allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                           const ConstVkNextProxy<vk::DescriptorSetAllocateInfo>& next = {}) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkFreeDescriptorSets.html">vkFreeDescriptorSets</a>
   */
  vk::ResultValueType<void>::type freeDescriptorSets(const std::vector<DescriptorSet>& descriptorSets) const;

  /**
   * @brief Reference: <a href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkResetDescriptorPool.html">vkResetDescriptorPool</a>
   */
  vk::ResultValueType<void>::type
    reset(const vk::DescriptorPoolResetFlags& flags = vk::DescriptorPoolResetFlags()) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::DescriptorPool&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_DESCRIPTOR_DESCRIPTOR_POOL_HPP
