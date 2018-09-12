/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_EXCEPTIONS_H
#define BASE_EXCEPTIONS_H
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
class IllegalInvocation : LogiException {
public:
	/**
	 * @brief	Forwards message to the std exception class.
	 *
	 * @param	message Exception message.
	 */
    explicit IllegalInvocation(const std::string& message);

	/**
	 * @brief	Forwards message to the std exception class.
	 *
	 * @param	message Exception message.
	 */
    explicit IllegalInvocation(const char* message);
};

/**
 * @brief   InitializationError error.
 */
class InitializationError : LogiException {
public:
	/**
	 * @brief	Forwards message to the std exception class.
	 *
	 * @param	message Exception message.
	 */
	explicit InitializationError(const std::string& message);

	/**
	 * @brief	Forwards message to the std exception class.
	 *
	 * @param	message Exception message.
	 */
	explicit InitializationError(const char* message);
};

}

#endif