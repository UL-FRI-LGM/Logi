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

EventImpl::EventImpl(LogicalDeviceImpl& logical_device, const vk::EventCreateInfo& create_info,
                     const std::optional<vk::AllocationCallbacks>& allocator)
  : logical_device_(logical_device), allocator_(allocator) {
  vk::Device vk_device = logical_device_;
  vk_event_ = vk_device.createEvent(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

bool EventImpl::getStatus() const {
  vk::Device vk_device;
  return vk_device.getEventStatus(vk_event_, getDispatcher()) == vk::Result::eSuccess;
}

vk::ResultValueType<void>::type EventImpl::reset() const {
  vk::Device vk_device;
  return vk_device.resetEvent(vk_event_, getDispatcher());
}

vk::ResultValueType<void>::type EventImpl::set() const {
  vk::Device vk_device;
  return vk_device.setEvent(vk_event_, getDispatcher());
}

VulkanInstanceImpl& EventImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& EventImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& EventImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& EventImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

void EventImpl::destroy() const {
  // TODO
}

EventImpl::operator const vk::Event() const {
  return vk_event_;
}

void EventImpl::free() {
  vk::Device vk_device = logical_device_;
  vk_device.destroy(vk_event_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
