/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_TEXTURE_H_
#define _VKR_VK_TEXTURE_H_

#include <vulkan/vulkan.h>
#include "vk_device.h"

// Forward declaration due to the circular dependency.
class VulkanDevice;

namespace vkr {

class VulkanTexture {
public:
	/**
	 * @brief Creates new VulkanTexture wrapper that wraps the given VkImage. 
	 *
	 * @param image VkImage that will be wrapped.
	 * @param owner Owner device.
	 * @param manage_dealloc If true the texture will manage VkImage deallocation.
	 */
	explicit VulkanTexture(VkImage image, VulkanDevice* owner, bool manage_dealloc);

	/** 
	 * @brief Deallocates VkImage if the owner is specified.
	 */
	~VulkanTexture();

private:
	VkImage image_; ///< VkImage handle.
	VulkanDevice* owner_; ///< VkImage memory allocation owner.
	bool manage_dealloc_; ///< True if the texture is responsible for VkImage deallocation.
};

}
#endif //  _VKR_VK_TEXTURE_H_