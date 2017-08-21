/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_INSTANCE_MANAGER_H_
#define _VKR_VK_INSTANCE_MANAGER_H_

#include <vulkan/vulkan.hpp>
#include <cstdint>
#include <vector>

namespace vkr {

/**
 * @brief Class used to manage Vulkan instance.
 */
class VulkanInstanceManager {
public:
	/**
	 * @brief Creates and initializes Vulkan instance.
	 *
	 * @param extensions Names of the requested extensions.
	 */
	VulkanInstanceManager(std::vector<char *> extensions);

	/**
	* @brief Cleans up.
	*/
	~VulkanInstanceManager();

protected:
	/**
	 * @brief Checks if all of the requested validation layers are supported.
	 * 
	 * @param layers Vector containing names of the requested validation layers.
	 * @return true if all the layers are supported.
	 */
	static bool checkValidationLayerSupport(const std::vector<char*> layers);

	/**
	 * @brief Sets up validation layers debug callback if validation is enabled. 
	 */
	void setupDebugCallback();

private:
	/**
	 * @brief Debug callback.
	 */
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

	VkInstance vk_instance_;  ///< Vulkan instance.

	VkDebugReportCallbackEXT callback;  ///< Debug callback.
};

}


#endif //  _VKR_VK_INSTANCE_MANAGER_H_
