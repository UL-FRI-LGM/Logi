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

#include "logi/descriptor/descriptor_pool_impl.hpp"
#include "logi/descriptor/descriptor_set_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

DescriptorPoolImpl::DescriptorPoolImpl(LogicalDeviceImpl& logicalDevice, const vk::DescriptorPoolCreateInfo& createInfo,
                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDescriptorPool_ =
    vkDevice.createDescriptorPool(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

// region Vulkan Declarations

vk::ResultValueType<void>::type DescriptorPoolImpl::reset(const vk::DescriptorPoolResetFlags& flags) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.resetDescriptorPool(vkDescriptorPool_, flags, getDispatcher());
}

std::vector<std::shared_ptr<DescriptorSetImpl>>
  DescriptorPoolImpl::allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
                                             const ConstVkNextProxy<vk::DescriptorSetAllocateInfo>& next) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vk::DescriptorSetAllocateInfo allocateInfo(vkDescriptorPool_, static_cast<uint32_t>(descriptorSetLayouts.size()),
                                             descriptorSetLayouts.data());
  allocateInfo.pNext = next;

  std::vector<vk::DescriptorSet> descriptorSets = vkDevice.allocateDescriptorSets(allocateInfo, getDispatcher());
  std::vector<std::shared_ptr<DescriptorSetImpl>> logiDescriptorSets;
  logiDescriptorSets.reserve(descriptorSets.size());

  for (const auto& set : descriptorSets) {
    logiDescriptorSets.emplace_back(VulkanObjectComposite<DescriptorSetImpl>::createObject(*this, set));
  }

  return logiDescriptorSets;
}

vk::ResultValueType<void>::type DescriptorPoolImpl::freeDescriptorSets(const std::vector<size_t>& descriptorSetIds) {
  std::vector<vk::DescriptorSet> vkDescriptorSets;
  vkDescriptorSets.reserve(descriptorSetIds.size());

  // Collect VK handles and destroy logi command buffers.
  for (size_t id : descriptorSetIds) {
    vkDescriptorSets.emplace_back(
      static_cast<const vk::DescriptorSet&>(*VulkanObjectComposite<DescriptorSetImpl>::getObject(id).get()));
    VulkanObjectComposite<DescriptorSetImpl>::destroyObject(id);
  }

  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.freeDescriptorSets(vkDescriptorPool_, vkDescriptorSets, getDispatcher());
}

// endregion

// region Logi Declarations

VulkanInstanceImpl& DescriptorPoolImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& DescriptorPoolImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& DescriptorPoolImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& DescriptorPoolImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void DescriptorPoolImpl::destroy() const {
  logicalDevice_.destroyDescriptorPool(id());
}

DescriptorPoolImpl::operator const vk::DescriptorPool&() const {
  return vkDescriptorPool_;
}

void DescriptorPoolImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkDescriptorPool_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

// endregion

} // namespace logi
