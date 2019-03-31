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

#include "logi/base/exception.hpp"

namespace logi {

LogiException::LogiException(const std::string& message) : std::runtime_error(message) {}

LogiException::LogiException(const char* const message) : std::runtime_error(message) {}

VulkanError::VulkanError(const vk::Result result, const std::string& message)
  : LogiException(message), result_(result) {}

VulkanError::VulkanError(const vk::Result result, const char* const message)
  : LogiException(message), result_(result) {}

vk::Result VulkanError::result() const {
  return result_;
}

IllegalInvocation::IllegalInvocation(const std::string& message) : LogiException(message) {}

IllegalInvocation::IllegalInvocation(const char* const message) : LogiException(message) {}

InitializationError::InitializationError(const std::string& message) : LogiException(message) {}

InitializationError::InitializationError(const char* const message) : LogiException(message) {}

} // namespace logi
