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

#include "logi/descriptor/descriptor_set.hpp"
#include "logi/descriptor/descriptor_pool.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"

namespace logi {

// region Vulkan Definitions

void DescriptorSet::updateWithTemplate(vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const {
  return object_->updateWithTemplate(descriptorUpdateTemplate, pData);
}

// endregion

// region Logi Definitions

VulkanInstance DescriptorSet::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice DescriptorSet::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice DescriptorSet::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

DescriptorPool DescriptorSet::getDescriptorPool() const {
  return DescriptorPool(object_->getDescriptorPool().shared_from_this());
}

const vk::DispatchLoaderDynamic& DescriptorSet::getDispatcher() const {
  return object_->getDispatcher();
}

void DescriptorSet::destroy() const {
  object_->destroy();
}

DescriptorSet::operator const vk::DescriptorSet&() const {
  static vk::DescriptorSet nullHandle(nullptr);
return (object_) ? object_->operator const vk::DescriptorSet&() : nullHandle;
}

// endregion

} // namespace logi
