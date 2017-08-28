/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_QUEUE_FAMILY_H_
#define _VKR_VK_QUEUE_FAMILY_H_

#include <vulkan/vulkan.hpp>
#include "vk_device.h"
#include "vk_queue.h"
#include <vector>

namespace vkr {

/**
 * @brief Forward declaration due to the circular dependency.
 */
class VulkanDevice;
class VulkanQueue;

/**
* @brief TODO
*/
class VulkanQueueFamily {
public:
	/**
	 * @brief Creates queue wrappers for this queue family.
	 */
	VulkanQueueFamily(VulkanDevice &owner_device, uint32_t family_idx, VkQueueFamilyProperties properties);

	/**
	 * @brief Initializes queues. Must be called after the device is initialized.
	 */
	void initializeQueues();

	/**
	 * @brief Initializes queue family command pool using the given flags.
	 * @throws std::runtime_error If method fails to create command pool.
	 */
	void initializeCommandPool(VkCommandPoolCreateFlags flags);

	/**
	* @brief Checks if the given queue family supports presentation.
	*
	* @return true if presentation is supported.
	*/
	bool presentationSupport(VkSurfaceKHR surface) const;

	/**
	 * @brief Queue family index getter.
	 *
	 * @return Index of the queue family.
	 */
	uint32_t getFamilyIdx() const;

	/**
	* @brief Queue count getter.
	*
	* @return Number of queues.
	*/
	uint32_t getQueueCount() const;

private:
	VulkanDevice &owner_device_; ///< Reference to the device owning this family queue.

	uint32_t family_idx_; ///< Index of the queue family.
	VkQueueFamilyProperties properties_; ///< Queue family properties.

	std::vector<std::shared_ptr<VulkanQueue>> queues_; ///< Vulkan queues.

	VkCommandPool command_pool_; ///< Queue family command pool.
};

}


#endif //  _VKR_VK_QUEUE_H_
