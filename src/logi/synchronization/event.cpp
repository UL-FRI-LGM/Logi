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

#include "logi/synchronization/event.hpp"
#include "logi/device/logical_device_impl.hpp"
#include "logi/device/physical_device_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"
#include "logi/synchronization/event_impl.hpp"

namespace logi {

bool Event::getStatus() const {
  return object_->getStatus();
}

vk::ResultValueType<void>::type Event::reset() const {
  return object_->reset();
}

vk::ResultValueType<void>::type Event::set() const {
  return object_->set();
}

VulkanInstance Event::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

PhysicalDevice Event::getPhysicalDevice() const {
  return PhysicalDevice(object_->getPhysicalDevice().shared_from_this());
}

LogicalDevice Event::getLogicalDevice() const {
  return LogicalDevice(object_->getLogicalDevice().shared_from_this());
}

const vk::DispatchLoaderDynamic& Event::getDispatcher() const {
  return object_->getDispatcher();
}

void Event::destroy() const {
  if (object_) {
    object_->destroy();
  }
}

Event::operator const vk::Event&() const {
  static vk::Event nullHandle(nullptr);
  return (object_) ? object_->operator const vk::Event&() : nullHandle;
}

} // namespace logi
