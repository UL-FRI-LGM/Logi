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

#include "logi/nvidia/object_table_nvx_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

ObjectTableNVXImpl::ObjectTableNVXImpl(LogicalDeviceImpl& logical_device,
                                       const vk::ObjectTableCreateInfoNVX& create_info,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : logical_device_(logical_device), allocator_(allocator) {
  vk::Device vk_device = logical_device_;
  vk_object_table_nvx =
    vk_device.createObjectTableNVX(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

vk::ResultValueType<void>::type
  ObjectTableNVXImpl::registerObjectsNVX(vk::ArrayProxy<const vk::ObjectTableEntryNVX* const> object_table_entries,
                                         vk::ArrayProxy<const uint32_t> object_indices) const {
  vk::Device vk_device;
  vk_device.registerObjectsNVX(vk_object_table_nvx, object_table_entries, object_indices, getDispatcher());
}

vk::ResultValueType<void>::type
  ObjectTableNVXImpl::unregisterObjectsNVX(vk::ArrayProxy<const vk::ObjectEntryTypeNVX> object_entry_types,
                                           vk::ArrayProxy<const uint32_t> object_indices) const {
  vk::Device vk_device;
  vk_device.unregisterObjectsNVX(vk_object_table_nvx, object_entry_types, object_indices, getDispatcher());
}

VulkanInstanceImpl& ObjectTableNVXImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& ObjectTableNVXImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& ObjectTableNVXImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& ObjectTableNVXImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

void ObjectTableNVXImpl::destroy() const {
  // TODO
}

ObjectTableNVXImpl::operator vk::ObjectTableNVX() const {
  return vk_object_table_nvx;
}

void ObjectTableNVXImpl::free() {
  vk::Device vk_device = logical_device_;
  vk_device.destroy(vk_object_table_nvx, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
