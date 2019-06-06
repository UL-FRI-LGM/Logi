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

#include "logi/program/pipeline_cache_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

PipelineCacheImpl::PipelineCacheImpl(LogicalDeviceImpl& logicalDevice, const vk::PipelineCacheCreateInfo& createInfo,
                                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  vkPipelineCache_ =
    vkDevice.createPipelineCache(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

typename vk::ResultValueType<std::vector<uint8_t>>::type PipelineCacheImpl::getPipelineCacheData() const {
  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.getPipelineCacheData(vkPipelineCache_, getDispatcher());
}

vk::ResultValueType<void>::type
  PipelineCacheImpl::mergeCaches(const vk::ArrayProxy<const vk::PipelineCache>& caches) const {
  auto vkDevice = static_cast<vk::Device>(getLogicalDevice());
  return vkDevice.mergePipelineCaches(vkPipelineCache_, caches, getDispatcher());
}

VulkanInstanceImpl& PipelineCacheImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& PipelineCacheImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& PipelineCacheImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& PipelineCacheImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void PipelineCacheImpl::destroy() const {
  logicalDevice_.destroyPipelineCache(id());
}

PipelineCacheImpl::operator vk::PipelineCache() const {
  return vkPipelineCache_;
}

void PipelineCacheImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkPipelineCache_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

}