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
#include "base/VulkanDevice.h"
#include "queues/QueueFamily.h"

namespace logi {

class SwapChain : public DependentDestroyableHandle {
public:
	/**
	 * @brief Default constructor that initializes SwapChain members to default values.
	 */
	SwapChain(std::weak_ptr<HandleManager>& owner, const vk::Device& device, vk::SurfaceKHR& surface, uint32_t present_family, const std::vector<uint32_t>& concurrent_families);

	/**
	 * @beiwf Set instance and device that will be used by the SwapChain and get all required function pointers
	 *

	 */
	void connect(const vk::Instance& instance, VulkanDevice device);

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
	vk::ResultValue<uint32_t> acquireNextImage(vk::Semaphore present_complete_semaphore = nullptr);

	/**
	 * @brief Queue an image for presentation.
	 *
	 * @param queue Presentation queue for presenting the image.
	 * @param image_index Index of the swap chain image to queue for presentation.
	 * @param wait_semaphore (Optional) Semaphore that is waited on before the image is presented.
	 *
	 * @return Result of the queue presentation.
	 */
	vk::Result queuePresent(vk::Queue queue, uint32_t image_index, vk::Semaphore wait_semaphore = nullptr);

	/**
	 * @brief Clean up swap chain data, surface data and disconnect from the device.
	 */
	void cleanup();

	/**
	 * @brief Default destructor that calls cleanup.
	 */
	~SwapChain();

private:
	struct SwapchainData {
		vk::Device device;
		vk::SurfaceKHR surface;					///< Surface.
		vk::SwapchainKHR swapchain;
		vk::Format color_format;				///< Selected swap chain color format.
		vk::ColorSpaceKHR color_space;			///< Selected swap chain color space.

		std::vector<vk::Image> images;			///< Swap chain images.
		std::vector<vk::ImageView> image_views;	///< Views of swap chain images
	};

	std::shared_ptr<SwapchainData> data_;
};

}
#endif SWAPCHAIN_H_