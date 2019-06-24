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

#include "logi/program/validation_cache_ext_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

ValidationCacheEXTImpl::ValidationCacheEXTImpl(LogicalDeviceImpl& logicalDevice,
                                               const vk::ValidationCacheCreateInfoEXT& createInfo,
                                               const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  vkValidationCacheEXT_ =
    vkDevice.createValidationCacheEXT(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Definitions

vk::ResultValueType<void>::type
  ValidationCacheEXTImpl::mergeValidationCachesEXT(vk::ArrayProxy<const vk::ValidationCacheEXT> srcCaches) const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.mergeValidationCachesEXT(vkValidationCacheEXT_, srcCaches, getDispatcher());
}

typename vk::ResultValueType<std::vector<uint8_t>>::type ValidationCacheEXTImpl::getValidationCacheDataEXT() const {
  vk::Device vkDevice = logicalDevice_;
  return vkDevice.getValidationCacheDataEXT(vkValidationCacheEXT_, getDispatcher());
}

// endregion

// region Logi Definitions

VulkanInstanceImpl& ValidationCacheEXTImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& ValidationCacheEXTImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& ValidationCacheEXTImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& ValidationCacheEXTImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void ValidationCacheEXTImpl::destroy() const {
  logicalDevice_.destroyValidationCacheEXT(id());
}

ValidationCacheEXTImpl::operator vk::ValidationCacheEXT() const {
  return vkValidationCacheEXT_;
}

void ValidationCacheEXTImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkValidationCacheEXT_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
