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

#ifndef LOGI_BASE_DEBUG_UTILS_MESSENGER_EXT_IMPL_HPP
#define LOGI_BASE_DEBUG_UTILS_MESSENGER_EXT_IMPL_HPP

#include <optional>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;

class DebugUtilsMessengerEXTImpl : public VulkanObject,
                                   public std::enable_shared_from_this<DebugUtilsMessengerEXTImpl> {
 public:
  DebugUtilsMessengerEXTImpl(VulkanInstanceImpl& instance, const vk::DebugUtilsMessengerCreateInfoEXT& createInfo,
                             const std::optional<vk::AllocationCallbacks>& allocator);

  VulkanInstanceImpl& getInstance() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::DebugUtilsMessengerEXT&() const;

 protected:
  void free() override;

 private:
  VulkanInstanceImpl& instance_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::DebugUtilsMessengerEXT vkMessenger_;
};

} // namespace logi

#endif // LOGI_BASE_DEBUG_UTILS_MESSENGER_EXT_IMPL_HPP
