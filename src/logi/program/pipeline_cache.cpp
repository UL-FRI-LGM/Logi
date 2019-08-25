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

#include "logi/program/pipeline_cache.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"

namespace logi {

typename vk::ResultValueType<std::vector<uint8_t>>::type PipelineCache::getPipelineCacheData() const {
  return object_->getPipelineCacheData();
}

vk::ResultValueType<void>::type
  PipelineCache::mergeCaches(const vk::ArrayProxy<const vk::PipelineCache>& caches) const {
  return object_->mergeCaches(caches);
}

VulkanInstance PipelineCache::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice PipelineCache::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice PipelineCache::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& PipelineCache::getDispatcher() const {
  return object_->getDispatcher();
}

void PipelineCache::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

PipelineCache::operator const vk::PipelineCache&() const {
  static vk::PipelineCache nullHandle(nullptr);
  return (object_) ? object_->operator const vk::PipelineCache&() : nullHandle;
}

} // namespace logi