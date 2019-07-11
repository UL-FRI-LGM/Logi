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

#ifndef LOGI_PROGRAM_VALIDATION_CACHE_EXT_IMPL_HPP
#define LOGI_PROGRAM_VALIDATION_CACHE_EXT_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

class ValidationCacheEXTImpl : public VulkanObject, public std::enable_shared_from_this<ValidationCacheEXTImpl> {
 public:
  ValidationCacheEXTImpl(LogicalDeviceImpl& logicalDevice, const vk::ValidationCacheCreateInfoEXT& createInfo,
                         const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Commands

  vk::ResultValueType<void>::type
    mergeValidationCachesEXT(vk::ArrayProxy<const vk::ValidationCacheEXT> srcCaches) const;

  typename vk::ResultValueType<std::vector<uint8_t>>::type getValidationCacheDataEXT() const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::ValidationCacheEXT&() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::ValidationCacheEXT vkValidationCacheEXT_;
};

} // namespace logi

#endif // LOGI_PROGRAM_VALIDATION_CACHE_EXT_IMPL_HPP
