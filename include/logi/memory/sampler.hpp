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

#ifndef LOGI_MEMORY_SAMPLER_HPP
#define LOGI_MEMORY_SAMPLER_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"
#include "logi/memory/sampler_impl.hpp"

namespace logi {

class VulkanInstance;
class PhysicalDevice;
class LogicalDevice;

class Sampler : public Handle<SamplerImpl> {
 public:
  using Handle::Handle;

  // region Logi Declarations

  VulkanInstance getInstance() const;

  PhysicalDevice getPhysicalDevice() const;

  LogicalDevice getLogicalDevice() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::Sampler&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_MEMORY_SAMPLER_HPP
