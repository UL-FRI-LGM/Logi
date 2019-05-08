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

#include "logi/synchronization/event_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

EventImpl::EventImpl(LogicalDeviceImpl& logicalDevice, const vk::EventCreateInfo& createInfo,
                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logicalDevice_(logicalDevice), allocator_(allocator) {
  vk::Device vkDevice = logicalDevice_;
  vkEvent_ = vkDevice.createEvent(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

bool EventImpl::getStatus() const {
  vk::Device vkDevice;
  return vkDevice.getEventStatus(vkEvent_, getDispatcher()) == vk::Result::eSuccess;
}

vk::ResultValueType<void>::type EventImpl::reset() const {
  vk::Device vkDevice;
  return vkDevice.resetEvent(vkEvent_, getDispatcher());
}

vk::ResultValueType<void>::type EventImpl::set() const {
  vk::Device vkDevice;
  return vkDevice.setEvent(vkEvent_, getDispatcher());
}

VulkanInstanceImpl& EventImpl::getInstance() const {
  return logicalDevice_.getInstance();
}

PhysicalDeviceImpl& EventImpl::getPhysicalDevice() const {
  return logicalDevice_.getPhysicalDevice();
}

LogicalDeviceImpl& EventImpl::getLogicalDevice() const {
  return logicalDevice_;
}

const vk::DispatchLoaderDynamic& EventImpl::getDispatcher() const {
  return logicalDevice_.getDispatcher();
}

void EventImpl::destroy() const {
  // TODO
}

EventImpl::operator vk::Event() const {
  return vkEvent_;
}

void EventImpl::free() {
  vk::Device vkDevice = logicalDevice_;
  vkDevice.destroy(vkEvent_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
