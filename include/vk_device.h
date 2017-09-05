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
#include "vk_queue_family.h"
#include <array>
#include <vector>
#include <vk_mem_alloc.h>

namespace vkr {

/**
* @brief Forward declaration due to the circular dependency.
*/
class VulkanQueueFamily;

/**
* @brief TODO
*/
class VulkanDevice {

public:
	/**
	 * @brief Wraps VkPhysicalDevice and retrieves meta data for the device.
	 *
	 * @param Physical device handle.
	 * @param Index of the physical device.
	 */
	VulkanDevice(VkPhysicalDevice device);

	/**
	 * Cleanup.
	 */
	~VulkanDevice();

	/**
	 * @brief Checks if the device supports the extensions in the given vector.
	 *
	 * @param extensions List of extensions to check.
	 * @return true if all of the extensions are supported. 
	 */
	bool supportsExtensions(const std::vector<const char*> &extensions) const;

	/**
	 * @brief Creates new logical device with the requested extensions and features. It also creates all graphical queues for the device. Logical device must be initialized
	 * before performing other operations.
	 *
	 * @param requested_extensions Extensions that will be enabled.
	 * @param requested_features Features that will be enabled.
	 * @return true if the logical device was successfully created.
	 */
	bool initializeLogicalDevice(const std::vector<const char*> &requested_extensions, const VkPhysicalDeviceFeatures *requested_features);

	/**
	* @brief Wait for the device to become idle.
	*/
	void VulkanDevice::waitIdle() const;

	/**
	* @brief Checks if the device has graphics capabilities.
	*
	* @return true if the device has graphics capabilities.
	*/
	bool hasGraphicsCapabilities() const;

	/**
	* @brief Checks if the device has compute capabilities.
	*
	* @return true if the device has compute capabilities.
	*/
	bool hasComputeCapabilities() const;

	/**
	* @brief Returns structure that marks which features are supported.
	*
	* @return VkPhysicalDeviceFeatures.
	*/
	VkPhysicalDeviceFeatures getSupportedFeatures() const;

	/**
	* @brief Returns physical device handle.
	*
	* @return VkDevice Physical device handle.
	*/
	VkPhysicalDevice getPhysicalDeviceHandle();

	/**
	 * @brief Returns logical device handle.
	 *
	 * @return VkDevice Logical device handle.
	 */
	VkDevice getLogicalDeviceHandle();

private:
	VkPhysicalDevice physical_device_; ///< Physical device handle.
	VkDevice logical_device_; ///< Logical device handle.

	VkPhysicalDeviceProperties device_properties_; ///< Structure specifying physical device properties.
	VkPhysicalDeviceFeatures device_features_; ///< Structure describing the fine-grained features that can be supported by an implementation.
	VkPhysicalDeviceMemoryProperties memory_properties_; ///< Structure specifying physical device memory properties.

	std::vector<VkExtensionProperties> available_extensions_; ///< List of supported extensions.

	std::vector<std::shared_ptr<VulkanQueueFamily>> graphics_queue_families_; ///< Vector containing all queue families that have graphics capabilities (and implicitly compute capabilities).
	std::vector<std::shared_ptr<VulkanQueueFamily>> compute_queue_families_; ///< Vector containing all queue families that have compute capabilities but not graphics capabilities.
	std::vector<std::shared_ptr<VulkanQueueFamily>> transfer_queue_families_; ///< Vector containing all queue families that have dedicated transfer capabilities.

	VmaAllocator mem_allocator_; ///< Device memory allocator.

	bool initialized_; ///< True if the logical device was successfully initialized.
};

}


#endif //  _VKR_VK_DEVICE_H_
