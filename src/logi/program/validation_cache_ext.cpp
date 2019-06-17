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

#include "logi/program/validation_cache_ext.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"

namespace logi {

// region Vulkan Definitions

vk::ResultValueType<void>::type
  ValidationCacheEXT::mergeValidationCachesEXT(vk::ArrayProxy<const vk::ValidationCacheEXT> srcCaches) const {
  return object_->mergeValidationCachesEXT(srcCaches);
}

typename vk::ResultValueType<std::vector<uint8_t>>::type ValidationCacheEXT::getValidationCacheDataEXT() const {
  return object_->getValidationCacheDataEXT();
}

// endregion

// region Logi Definitions

VulkanInstance ValidationCacheEXT::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice ValidationCacheEXT::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice ValidationCacheEXT::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& ValidationCacheEXT::getDispatcher() const {
  return object_->getDispatcher();
}

void ValidationCacheEXT::destroy() const {
  object_->destroy();
}

ValidationCacheEXT::operator vk::ValidationCacheEXT() const {
  return object_->operator vk::ValidationCacheEXT();
}

// endregion

} // namespace logi
