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

#ifndef LOGI_SURFACE_SURFACE_KHR_HPP
#define LOGI_SURFACE_SURFACE_KHR_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"

namespace logi {

class VulkanInstance;
class SurfaceKHRImpl;

class SurfaceKHR : public Handle<SurfaceKHRImpl> {
 public:
  using Handle::Handle;

  // region Logi Declarations

  VulkanInstance getInstance() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::SurfaceKHR&() const;

  // endregion
};

} // namespace logi

#endif // LOGI_SURFACE_SURFACE_KHR_HPP
