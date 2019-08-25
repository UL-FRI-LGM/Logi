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

#include "logi/descriptor/descriptor_update_template.hpp"
#include "logi/descriptor/descriptor_update_template_impl.hpp"
#include "logi/device/logical_device.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/vulkan_instance.hpp"

namespace logi {

// region Logi Definitions

VulkanInstance DescriptorUpdateTemplate::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice DescriptorUpdateTemplate::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice DescriptorUpdateTemplate::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& DescriptorUpdateTemplate::getDispatcher() const {
  return object_->getDispatcher();
}

void DescriptorUpdateTemplate::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

DescriptorUpdateTemplate::operator const vk::DescriptorUpdateTemplate&() const {
  static vk::DescriptorUpdateTemplate nullHandle(nullptr);
  return (object_) ? object_->operator const vk::DescriptorUpdateTemplate&() : nullHandle;
}

// endregion

} // namespace logi
