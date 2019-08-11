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

#ifndef LOGI_QUERY_QUERY_POOL_IMPL_HPP
#define LOGI_QUERY_QUERY_POOL_IMPL_HPP

#include <vulkan/vulkan.hpp>
#include <optional>
#include "logi/base/vulkan_object.hpp"
#include "logi/device/logical_device_impl.hpp"

namespace logi {

class VulkanInstanceImpl;
class PhysicalDeviceImpl;
class LogicalDeviceImpl;

class QueryPoolImpl : public VulkanObject, public std::enable_shared_from_this<QueryPoolImpl> {
 public:
  QueryPoolImpl(LogicalDeviceImpl& logicalDevice, const vk::QueryPoolCreateInfo& createInfo,
                const std::optional<vk::AllocationCallbacks>& allocator = {});

  // region Vulkan Declarations

  template <typename T>
  vk::Result getResults(uint32_t firstQuery, uint32_t queryCount, vk::ArrayProxy<T> data, vk::DeviceSize stride,
                        vk::QueryResultFlags flags) const;

  void resetQueryPoolEXT(uint32_t firstQuery, uint32_t queryCount) const;

  // endregion

  // region Logi Declarations

  VulkanInstanceImpl& getInstance() const;

  PhysicalDeviceImpl& getPhysicalDevice() const;

  LogicalDeviceImpl& getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::QueryPool&() const;

 protected:
  void free() override;

  // endregion

 private:
  LogicalDeviceImpl& logicalDevice_;
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::QueryPool vkQueryPool_;
};

template <typename T>
vk::Result QueryPoolImpl::getResults(uint32_t firstQuery, uint32_t queryCount, vk::ArrayProxy<T> data,
                                     vk::DeviceSize stride, vk::QueryResultFlags flags) const {
  auto vkDevice = static_cast<vk::Device>(logicalDevice_);
  return vkDevice.getQueryPoolResults(vkQueryPool_, firstQuery, queryCount, data, stride, flags, getDispatcher());
}

} // namespace logi

#endif // LOGI_QUERY_QUERY_POOL_IMPL_HPP
