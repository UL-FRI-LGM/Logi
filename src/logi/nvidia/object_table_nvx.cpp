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

#include "logi/nvidia/object_table_nvx.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/nvidia/object_table_nvx_impl.hpp"

namespace logi {

vk::ResultValueType<void>::type
  ObjectTableNVX::registerObjectsNVX(vk::ArrayProxy<const vk::ObjectTableEntryNVX* const> object_table_entries,
                                     vk::ArrayProxy<const uint32_t> object_indices) const {
  return object_->registerObjectsNVX(object_table_entries, object_indices);
}

vk::ResultValueType<void>::type
  ObjectTableNVX::unregisterObjectsNVX(vk::ArrayProxy<const vk::ObjectEntryTypeNVX> object_entry_types,
                                       vk::ArrayProxy<const uint32_t> object_indices) const {
  return object_->unregisterObjectsNVX(object_entry_types, object_indices);
}

VulkanInstance ObjectTableNVX::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice ObjectTableNVX::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice ObjectTableNVX::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& ObjectTableNVX::getDispatcher() const {
  return object_->getDispatcher();
}

void ObjectTableNVX::destroy() const {
  object_->destroy();
}

ObjectTableNVX::operator vk::ObjectTableNVX() const {
  return object_->operator vk::ObjectTableNVX();
}

} // namespace logi
