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

#include "logi/nvidia/indirect_commands_layout_ndx_impl.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

IndirectCommandsLayoutNVXImpl::IndirectCommandsLayoutNVXImpl(LogicalDeviceImpl& logical_device,
                                                             const vk::IndirectCommandsLayoutCreateInfoNVX& create_info,
                                                             const std::optional<vk::AllocationCallbacks>& allocator)
  : logical_device_(logical_device), allocator_(allocator) {
  vk::Device vk_device = logical_device_;
  vk_indirect_commands_layout_nvx =
    vk_device.createIndirectCommandsLayoutNVX(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& IndirectCommandsLayoutNVXImpl::getInstance() const {
  return logical_device_.getInstance();
}

PhysicalDeviceImpl& IndirectCommandsLayoutNVXImpl::getPhysicalDevice() const {
  return logical_device_.getPhysicalDevice();
}

LogicalDeviceImpl& IndirectCommandsLayoutNVXImpl::getLogicalDevice() const {
  return logical_device_;
}

const vk::DispatchLoaderDynamic& IndirectCommandsLayoutNVXImpl::getDispatcher() const {
  return logical_device_.getDispatcher();
}

void IndirectCommandsLayoutNVXImpl::destroy() const {
  // TODO
}

IndirectCommandsLayoutNVXImpl::operator vk::IndirectCommandsLayoutNVX() const {
  return vk_indirect_commands_layout_nvx;
}

void IndirectCommandsLayoutNVXImpl::free() {
  vk::Device vk_device = logical_device_;
  vk_device.destroy(vk_indirect_commands_layout_nvx, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  VulkanObject::free();
}

} // namespace logi
