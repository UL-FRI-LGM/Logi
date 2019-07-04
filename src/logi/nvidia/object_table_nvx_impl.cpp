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

ObjectTableNVXImpl::ObjectTableNVXImpl(LogicalDeviceImpl& logicalDevice, const vk::ObjectTableCreateInfoNVX& createInfo,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkObjectTableNVX_ =
    vkDevice.createObjectTableNVX(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Definitions

vk::ResultValueType<void>::type
  ObjectTableNVXImpl::registerObjectsNVX(vk::ArrayProxy<const vk::ObjectTableEntryNVX* const> objectTableEntries,
                                         vk::ArrayProxy<const uint32_t> objectIndices) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.registerObjectsNVX(vkObjectTableNVX_, objectTableEntries, objectIndices, getDispatcher());
}

vk::ResultValueType<void>::type
  ObjectTableNVXImpl::unregisterObjectsNVX(vk::ArrayProxy<const vk::ObjectEntryTypeNVX> objectEntryTypes,
                                           vk::ArrayProxy<const uint32_t> objectIndices) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.unregisterObjectsNVX(vkObjectTableNVX_, objectEntryTypes, objectIndices, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& ObjectTableNVXImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& ObjectTableNVXImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& ObjectTableNVXImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& ObjectTableNVXImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void ObjectTableNVXImpl::destroy() const {
  return logicalDevice_.destroyObjectTableNVX(id());
}

ObjectTableNVXImpl::operator const vk::ObjectTableNVX&() const {
  return vkObjectTableNVX_;
}

void ObjectTableNVXImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkObjectTableNVX_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
