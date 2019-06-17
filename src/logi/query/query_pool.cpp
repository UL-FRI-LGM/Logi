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

#include "logi/query/query_pool.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/query/query_pool_impl.hpp"

namespace logi {

void QueryPool::resetQueryPoolEXT(uint32_t firstQuery, uint32_t queryCount) const {
  object_->resetQueryPoolEXT(firstQuery, queryCount);
}

VulkanInstance QueryPool::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice QueryPool::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice QueryPool::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& QueryPool::getDispatcher() const {
  return object_->getDispatcher();
}

void QueryPool::destroy() const {
  object_->destroy();
}

QueryPool::operator vk::QueryPool() const {
  return object_->operator vk::QueryPool();
}

} // namespace logi
