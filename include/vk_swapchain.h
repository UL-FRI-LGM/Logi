/*
* Project       : VulkanRenderer
*
* Author        : Primoz Lavric
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_SWAPCHAIN_H_
#define _VKR_VK_SWAPCHAIN_H_

#include <vulkan/vulkan.hpp>
#include "vk_device.h"
#include <vector>

namespace vkr {

class VulkanSwapChain {
public:
	/**
	 * @brief Structure used in swap-chain support query.
	 */
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

	/**
	 * @brief Retrieves VulkanSwapchain singleton instance.
	 *
	 * @return VulkanSwapchain instance.
	 */
	static VulkanSwapChain& getInstance();

	/**
	 * @brief Copy constructor not supported in singleton.
	 */
	VulkanSwapChain(VulkanSwapChain const&) = delete;

	/**
	 * @brief Copy assignment operator not supported in singleton.
	 */
	void operator=(VulkanSwapChain const&) = delete;


	/**
	 * @brief Initializes swap chain for the given device and surface.
	 *
	 * @param device Device that will hold the swap-chain.
	 * @param surface Surface that will be used by swap-chain to present the images.
	 * @param width Surface width.
	 * @param height Surface height.
	 * @param color_format Color format used in presentation images.
	 * @param depth_format Depth format used for depth buffer. If VK_FORMAT_UNDEFINED depth buffer wont be used.
	 */
	void initialize(std::weak_ptr<VulkanDevice> device_weak, VkSurfaceKHR surface, uint32_t width, uint32_t height, bool vsync, VkFormat color_format, VkFormat depth_format);

	/**
	 * @brief Retrieves device capabilities, supported formats and supported present modes for the given surface.
	 *
	 * @param device Vulkan physical device handle.
	 * @param surface Vulkan surface for which the support will be checked.
	 * @return Swap-chain support details.
	 */
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	VulkanSwapChain() : initialized_(false) {}

	/**
	 * @brief Picks presentation mode based on the supported presentation modes and requested state of vsync.
	 *
	 * @param supported_present_modes Presentation modes supported by the device.
	 * @param vsync True if vertical synchronization is enabled.
	 * @return Selected presentation mode.
	 */
	VkPresentModeKHR pickSwapchainPresentMode(const std::vector<VkPresentModeKHR> &supported_present_modes, bool vsync);

	/**
	 * @brief Picks the surface format from the supported surface formats.
	 * 
	 * @param supported_surface_formats Surface formats supported by the device.
	 * @return Selected surface format.
	 */
	VkSurfaceFormatKHR pickSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &supported_surface_formats);

	bool initialized_;
	std::shared_ptr<VulkanDevice> device_;
	VkSwapchainKHR swapchain_;
};

}

#endif //  _VKR_VK_SWAPCHAIN_H_