/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
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

#include "logi/instance/debug_report_callback_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

DebugReportCallbackEXTImpl::DebugReportCallbackEXTImpl(VulkanInstanceImpl& instance,
                                                       const vk::DebugReportCallbackCreateInfoEXT& create_info,
                                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vk_instance = instance_;
  vk_callback_ =
    vk_instance.createDebugReportCallbackEXT(create_info, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& DebugReportCallbackEXTImpl::getInstance() const {
  return instance_;
}

const vk::DispatchLoaderDynamic& DebugReportCallbackEXTImpl::getDispatcher() const {
  return instance_.getDispatcher();
}

void DebugReportCallbackEXTImpl::destroy() const {
  instance_.destroyDebugReportCallbackEXT(id());
}

DebugReportCallbackEXTImpl::operator vk::DebugReportCallbackEXT() const {
  return vk_callback_;
}

void DebugReportCallbackEXTImpl::free() {
  vk::Instance vk_instance = instance_;
  vk_instance.destroyDebugReportCallbackEXT(vk_callback_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  vk_callback_ = nullptr;
  VulkanObject::free();
}

} // namespace logi