/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_QUEUE_H_
#define _VKR_VK_QUEUE_H_

#include <vulkan/vulkan.hpp>
#include "vk_queue_family.h"

namespace vkr {

/**
 * @brief Forward declaration due to the circular dependency.
 */
class VulkanQueueFamily;

/**
* @brief TODO
*/
class VulkanQueue {
public:
	VulkanQueue(VulkanQueueFamily &owner_queue_family, VkQueue queue);

	~VulkanQueue();

private:
	VulkanQueueFamily &owner_queue_family_;

	VkQueue queue_;
};

}


#endif //  _VKR_VK_QUEUE_H_
