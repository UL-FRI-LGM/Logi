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

#include "logi/descriptor/descriptor_pool.hpp"
#include "logi/descriptor/descriptor_set.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"

namespace logi {

// region Vulkan Definitions

std::vector<DescriptorSet>
  DescriptorPool::allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                                         const ConstVkNextProxy<vk::DescriptorSetAllocateInfo>& next) const {
  std::vector<std::shared_ptr<DescriptorSetImpl>> descriptorSetImpls =
    object_->allocateDescriptorSets(descriptorSetLayouts, next);
  return std::vector<DescriptorSet>(descriptorSetImpls.begin(), descriptorSetImpls.end());
}

vk::ResultValueType<void>::type
  DescriptorPool::freeDescriptorSets(const std::vector<DescriptorSet>& descriptorSets) const {
  std::vector<size_t> ids;
  ids.reserve(descriptorSets.size());

  for (const auto& set : descriptorSets) {
    ids.emplace_back(set.id());
  }

  return object_->freeDescriptorSets(ids);
}

vk::ResultValueType<void>::type DescriptorPool::reset(const vk::DescriptorPoolResetFlags& flags) const {
  return object_->reset(flags);
}

// endregion

// region Logi Definitions

VulkanInstance DescriptorPool::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice DescriptorPool::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice DescriptorPool::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& DescriptorPool::getDispatcher() const {
  return object_->getDispatcher();
}

void DescriptorPool::destroy() const {
  object_->destroy();
}

DescriptorPool::operator vk::DescriptorPool() const {
  return object_->operator vk::DescriptorPool();
}

// endregion

} // namespace logi
