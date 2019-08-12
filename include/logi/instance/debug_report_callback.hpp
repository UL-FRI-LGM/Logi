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

#ifndef LOGI_BASE_DEBUG_REPORT_CALLBACK_HPP
#define LOGI_BASE_DEBUG_REPORT_CALLBACK_HPP

#include "logi/base/common.hpp"
#include "logi/base/handle.hpp"

namespace logi {

class VulkanInstance;
class DebugReportCallbackEXTImpl;

class DebugReportCallbackEXT : public Handle<DebugReportCallbackEXTImpl> {
 public:
  using Handle::Handle;

  VulkanInstance getInstance() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  void destroy() const;

  operator const vk::DebugReportCallbackEXT&() const;
};

} // namespace logi

#endif // LOGI_DEBUG_REPORT_CALLBACK_HPP
