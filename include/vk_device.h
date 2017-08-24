/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_DEVICE_H_
#define _VKR_VK_DEVICE_H_

#include <vulkan/vulkan.hpp>
#include "vk_queue.h"
#include <array>
#include <vector>

namespace vkr {

/**
* @brief TODO
*/
class VulkanDevice {

public:
	VulkanDevice(VkPhysicalDevice device, uint32_t device_idx);

	/**
	 * Checks if the device supports the extensions in the given vector.
	 * @param extensions List of extensions to check.
	 * 
	 * @return true if all of the extensions are supported. 
	 */
	bool supportsExtensions(const std::vector<const char*> &extensions);

	/**
	 * Checks if the device has graphics capabilities.
	 * 
	 * @return ture if the device has graphics capabilities.
	 */
	inline bool hasGraphicsCapabilities() { return graphics_capabilities_; }

	bool initialize(const std::vector<const char*> &requested_extensions);

private:
	struct QueueInfo {
		uint32_t family_idx;
		std::vector<std::shared_ptr<VulkanQueue>> queues;
	};

	VkPhysicalDevice physical_device_;
	VkDevice logical_device_;
	uint32_t device_idx_;

	VkPhysicalDeviceProperties device_properties_;
	VkPhysicalDeviceFeatures device_features_;
	VkPhysicalDeviceMemoryProperties memory_properties_;

	bool graphics_capabilities_;
	std::vector<VkExtensionProperties> available_extensions_;

	std::vector<QueueInfo> graphics_queue_families_;
};

}


#endif //  _VKR_VK_DEVICE_H_
