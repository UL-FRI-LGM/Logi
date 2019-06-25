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

#include "logi/render_pass/render_pass_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

RenderPassImpl::RenderPassImpl(LogicalDeviceImpl& logicalDevice, const vk::RenderPassCreateInfo& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkRenderPass_ = vkDevice.createRenderPass(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

RenderPassImpl::RenderPassImpl(LogicalDeviceImpl& logicalDevice, const vk::RenderPassCreateInfo2KHR& createInfo,
                               const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkRenderPass_ =
    vkDevice.createRenderPass2KHR(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

vk::Extent2D RenderPassImpl::getRenderAreaGranularity() const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getRenderAreaGranularity(vkRenderPass_, getDispatcher());
}

VulkanInstanceImpl& RenderPassImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& RenderPassImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& RenderPassImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& RenderPassImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void RenderPassImpl::destroy() const {
  logicalDevice_.destroyRenderPass(id());
}

RenderPassImpl::operator const vk::RenderPass&() const {
  return vkRenderPass_;
}

void RenderPassImpl::free() {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  vkDevice.destroy(vkRenderPass_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

}