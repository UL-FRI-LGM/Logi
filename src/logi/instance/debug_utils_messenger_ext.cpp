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

#include "logi/instance/debug_utils_messenger_ext.hpp"
#include "logi/instance/debug_utils_messenger_ext_impl.hpp"
#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

VulkanInstance DebugUtilsMessengerEXT::getInstance() const {
  return VulkanInstance(object_->getInstance().shared_from_this());
}

const vk::DispatchLoaderDynamic& DebugUtilsMessengerEXT::getDispatcher() const {
  return object_->getDispatcher();
}

void DebugUtilsMessengerEXT::destroy() const {
  object_->destroy();
}

DebugUtilsMessengerEXT::operator const vk::DebugUtilsMessengerEXT&() const {
  return *object_;
}

} // namespace logi