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

#include "logi/descriptor/descriptor_update_template_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

DescriptorUpdateTemplateImpl::DescriptorUpdateTemplateImpl(LogicalDeviceImpl& logicalDevice,
                                                           const vk::DescriptorUpdateTemplateCreateInfo& createInfo,
                                                           const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDescriptorUpdateTemplate_ =
    vkDevice.createDescriptorUpdateTemplate(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Logi Definitions

VulkanInstanceImpl& DescriptorUpdateTemplateImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& DescriptorUpdateTemplateImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& DescriptorUpdateTemplateImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& DescriptorUpdateTemplateImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void DescriptorUpdateTemplateImpl::destroy() const {
  logicalDevice_.destroyDescriptorUpdateTemplate(id());
}

DescriptorUpdateTemplateImpl::operator const vk::DescriptorUpdateTemplate&() const {
  return vkDescriptorUpdateTemplate_;
}

void DescriptorUpdateTemplateImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkDescriptorUpdateTemplate_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
