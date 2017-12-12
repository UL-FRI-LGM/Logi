/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef SWAPCHAIN_H_
#define SWAPCHAIN_H_

#include <vulkan/vulkan.hpp>
#include <tuple>
#include "base/VulkanDevice.h"

namespace vkr {

class SwapChain {
public:
	/**
	 * @brief Default constructor that initializes SwapChain members to default values.
	 */
	SwapChain();

	/**
	 * @beiwf Set instance and device that will be used by the SwapChain and get all required function pointers
	 *
	 * @param instance Vulkan instance,
	 * @param device Pointer to vulkan device.
	 */
	void connect(vk::Instance& instance, VulkanDevice* device);

	/**
	 * @brief Store surface and initialize the SwapChain. Selects the present queue, color format and color space.
	 *
	 * @param surface Vulkan KHR surface.
	 */
	void init(vk::SurfaceKHR surface);

	/**
	 * @brief Create the SwapChain and retrieve images with the given width and height. Width and height may change to meet
	 * the surface requirements.
	 *
	 * @param width Requested SwapChain width.
	 * @param height Requested SwapChain height.
	 * @param vsync Should v-sync be enabled.
	 */
	void create(uint32_t& width, uint32_t& height, bool vsync = false);
	
	/**
	 * @brief Acquires the next image in the swap chain.
	 * @note The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX.
	 *
	 * @param present_complete_semaphore (Optional) Semaphore that is signaled when the image is ready for use.
	 * @return Tuple containing the result of the operation and index of the next image (valid if the operation was successful)
	 */
	std::tuple<vk::Result, uint32_t> acquireNextImage(vk::Semaphore present_complete_semaphore = nullptr);

	/**
	 * @brief Queue an image for presentation.
	 *
	 * @param queue Presentation queue for presenting the image.
	 * @param image_index Index of the swap chain image to queue for presentation.
	 * @param wait_semaphore (Optional) Semaphore that is waited on before the image is presented.
	 *
	 * @return Result of the queue presentation.
	 */
	vk::Result SwapChain::queuePresent(vk::Queue queue, uint32_t image_index, vk::Semaphore wait_semaphore = nullptr);

	/**
	 * @brief Clean up swap chain data, surface data and disconnect from the device.
	 */
	void cleanup();

	/**
	 * @brief Default destructor that calls cleanup.
	 */
	~SwapChain();

private:
	vk::Instance instance_; ///< Vulkan instance.
	VulkanDevice* device_; ///< Vulkan device.

	vk::SurfaceKHR surface_; ///< Surface.
	uint32_t present_family_idx_; ///< Index of queue family that supports presenting.

	vk::SwapchainKHR swapchain_; ///< Swap chain object.
	vk::Format color_format_; /// Selected swap chain color format.
	vk::ColorSpaceKHR color_space_; ///< Selected swap chain color space.

	// Color attachment images
	uint32_t image_count_; ///< Number of images inside the swap chain
	std::vector<vk::Image> images_; ///< Swap chain images.
	std::vector<vk::ImageView> image_views_; ///< Views of swap chain images

	// Function pointers.
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
	PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
	PFN_vkQueuePresentKHR fpQueuePresentKHR;
};

}

#endif SWAPCHAIN_H_