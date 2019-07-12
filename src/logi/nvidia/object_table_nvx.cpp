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

// region Vulkan Definitions

vk::ResultValueType<void>::type
  ObjectTableNVX::registerObjectsNVX(vk::ArrayProxy<const vk::ObjectTableEntryNVX* const> objectTableEntries,
                                     vk::ArrayProxy<const uint32_t> objectIndices) const {
  return object_->registerObjectsNVX(objectTableEntries, objectIndices);
}

vk::ResultValueType<void>::type
  ObjectTableNVX::unregisterObjectsNVX(vk::ArrayProxy<const vk::ObjectEntryTypeNVX> objectEntryTypes,
                                       vk::ArrayProxy<const uint32_t> objectIndices) const {
  return object_->unregisterObjectsNVX(objectEntryTypes, objectIndices);
}

// endregion

// region Logi Definitions

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

ObjectTableNVX::operator const vk::ObjectTableNVX&() const {
  static vk::ObjectTableNVX nullHandle(nullptr);
return (object_) ? object_->operator const vk::ObjectTableNVX&() : nullHandle;
}

// endregion

} // namespace logi
