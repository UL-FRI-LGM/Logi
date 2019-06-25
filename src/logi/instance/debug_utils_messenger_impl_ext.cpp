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

#include "logi/instance/debug_utils_messenger_ext_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

DebugUtilsMessengerEXTImpl::DebugUtilsMessengerEXTImpl(VulkanInstanceImpl& instance,
                                                       const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                                                       const std::optional<vk::AllocationCallbacks>& allocator)
  : instance_(instance), allocator_(allocator) {
  vk::Instance vkInstance = instance_;
  vkMessenger_ =
    vkInstance.createDebugUtilsMessengerEXT(createInfo, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
}

VulkanInstanceImpl& DebugUtilsMessengerEXTImpl::getInstance() const {
  return instance_;
}

const vk::DispatchLoaderDynamic& DebugUtilsMessengerEXTImpl::getDispatcher() const {
  return instance_.getDispatcher();
}

void DebugUtilsMessengerEXTImpl::destroy() const {
  instance_.destroyDebugUtilsMessengerEXT(*this);
}

DebugUtilsMessengerEXTImpl::operator const vk::DebugUtilsMessengerEXT&() const {
  return vkMessenger_;
}

void DebugUtilsMessengerEXTImpl::free() {
  vk::Instance vkInstance = instance_;
  vkInstance.destroyDebugUtilsMessengerEXT(vkMessenger_, allocator_ ? &allocator_.value() : nullptr, getDispatcher());
  vkMessenger_ = nullptr;
  VulkanObject::free();
}

} // namespace logi