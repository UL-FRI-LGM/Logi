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

#include "logi/descriptor/descriptor_set_impl.hpp"
#include "logi/descriptor/descriptor_pool_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

DescriptorSetImpl::DescriptorSetImpl(DescriptorPoolImpl& descriptorPool, const vk::DescriptorSet& vkDescriptorSet)
  : descriptorPool_(descriptorPool), vkDescriptorSet_(vkDescriptorSet) {}

// region Vulkan Definitions

void DescriptorSetImpl::updateWithTemplate(vk::DescriptorUpdateTemplate descriptorUpdateTemplate,
                                           const void* pData) const {
  vk::Device vkDevice = getLogicalDevice();
  vkDevice.updateDescriptorSetWithTemplate(vkDescriptorSet_, descriptorUpdateTemplate, pData, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& DescriptorSetImpl::getInstance() const {
  return descriptorPool_.getInstance();
}

PhysicalDeviceImpl& DescriptorSetImpl::getPhysicalDevice() const {
  return descriptorPool_.getPhysicalDevice();
}

LogicalDeviceImpl& DescriptorSetImpl::getLogicalDevice() const {
  return descriptorPool_.getLogicalDevice();
}

DescriptorPoolImpl& DescriptorSetImpl::getDescriptorPool() const {
  return descriptorPool_;
}

const vk::DispatchLoaderDynamic& DescriptorSetImpl::getDispatcher() const {
  return descriptorPool_.getDispatcher();
}

void DescriptorSetImpl::destroy() const {
  descriptorPool_.freeDescriptorSets({id()});
}

DescriptorSetImpl::operator vk::DescriptorSet() const {
  return vkDescriptorSet_;
}

void DescriptorSetImpl::free() {
  vkDescriptorSet_ = nullptr;
  VulkanObject::free();
}

// endregion

} // namespace logi
