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

#include "logi/render_pass/framebuffer_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

FramebufferImpl::FramebufferImpl(LogicalDeviceImpl& logicalDevice, const vk::FramebufferCreateInfo& createInfo,
                                 const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkFramebuffer_ = vkDevice.createFramebuffer(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& FramebufferImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& FramebufferImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& FramebufferImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& FramebufferImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void FramebufferImpl::destroy() const {
  // TODO
}

FramebufferImpl::operator vk::Framebuffer() const {
  return vkFramebuffer_;
}

void FramebufferImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkFramebuffer_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

}