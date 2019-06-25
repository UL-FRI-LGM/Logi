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

#ifndef LOGI_DESCRIPTOR_DESCRIPTOR_SET_IMPL_HPP
#define LOGI_DESCRIPTOR_DESCRIPTOR_SET_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class DescriptorPoolImpl;
class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

class DescriptorSetImpl : public VulkanObject<DescriptorSetImpl> {
 public:
  DescriptorSetImpl(DescriptorPoolImpl& descriptorPool, const vk::DescriptorSet& vkDescriptorSet);

  // region Vulkan Declarations

  void updateWithTemplate(vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  DescriptorPoolImpl& getDescriptorPool() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::DescriptorSet&() const;

 protected:
  void free() override;

  // endregion

 private:
  DescriptorPoolImpl& descriptorPool_;
  vk::DescriptorSet vkDescriptorSet_;
};

} // namespace logi

#endif // LOGI_DESCRIPTOR_DESCRIPTOR_SET_IMPL_HPP
