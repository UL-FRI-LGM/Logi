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

#ifndef LOGI_QUERY_QUERY_POOL_HPP
#define LOGI_QUERY_QUERY_POOL_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/query/query_pool_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class QueryPool : public Handle<QueryPoolImpl> {
 public:
  using Handle::Handle;

  // region Vulkan Declarations

  template <typename T>
  vk::Result getResults(uint32_t firstQuery, uint32_t queryCount, vk::ArrayProxy<T> data, vk::DeviceSize stride,
                        vk::QueryResultFlags flags) const;

  void resetQueryPoolEXT(uint32_t firstQuery, uint32_t queryCount) const;

  // endregion

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::QueryPool&() const;

  // endregion
};

template <typename T>
vk::Result QueryPool::getResults(uint32_t firstQuery, uint32_t queryCount, vk::ArrayProxy<T> data,
                                 vk::DeviceSize stride, vk::QueryResultFlags flags) const {
  return object_->getResults(firstQuery, queryCount, data, stride, flags);
}

} // namespace logi

#endif // LOGI_QUERY_QUERY_POOL_HPP
