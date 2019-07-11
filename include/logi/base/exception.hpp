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

#ifndef LOGI_BASE_EXCEPTION_HPP
#define LOGI_BASE_EXCEPTION_HPP

#include <stdexcept>
#include <vulkan/vulkan.hpp>

namespace logi {

/**
 * @brief   Base class for Logi framework exception.
 */
class LogiException : std::runtime_error {
 public:
  /**
   * @brief	Forwards message to the std exception class.
   *
   * @param	message Exception message.
   */
  explicit LogiException(const std::string& message);

  /**
   * @brief	Forwards message to the std exception class.
   *
   * @param	message Exception message.
   */
  explicit LogiException(const char* message);
};

/**
 * @brief   Vulkan error.
 */
class VulkanError : LogiException {
 public:
  /**
   * @brief	Forwards message to the std exception class and stores Vulkan result.
   *
   * @param   result  Error result value.
   * @param	message Exception message.
   */
  explicit VulkanError(vk::Result result, const std::string& message);

  /**
   * @brief	Forwards message to the std exception class and stores Vulkan result.
   *
   * @param   result  Error result value.
   * @param	message Exception message.
   */
  explicit VulkanError(vk::Result result, const char* message);

  /**
   * @brief   Retrieve the result
   *
   * @return  Result error.
   */
  vk::Result result() const;

 private:
  /**
   * Result returned by the Vulkan call that produced an error.
   */
  vk::Result result_;
};

/**
 * @brief   IllegalInvocation error.
 */
class IllegalInvocation : public LogiException {
 public:
  using LogiException::LogiException;
};

/**
 * @brief   BadAllocation error.
 */
class BadAllocation : public LogiException {
 public:
  using LogiException::LogiException;
};

/**
 * @brief   InitializationError error.
 */
class InitializationError : public LogiException {
 public:
  using LogiException::LogiException;
};

/**
 * @brief   ReflectionError error.
 */
class ReflectionError : public LogiException {
 public:
  using LogiException::LogiException;
};

} // namespace logi

#endif // LOGI_BASE_EXCEPTION_HPP