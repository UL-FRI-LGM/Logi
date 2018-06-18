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
#include "synchronization/Semaphore.h"
#include "memory/ImageView.h"
#include "queues/Queue.h"

namespace logi {

class SwapChain : public DependentDestroyableHandle {
public:
	/**
	 * @brief	Default placeholder constructor.
	 */
	SwapChain();

	/**
	 * @brief Default constructor that initializes SwapChain members to default values.
	 */
	SwapChain(std::weak_ptr<HandleManager>& owner, const vk::PhysicalDevice& physical_device, const vk::Device& device, const vk::SurfaceKHR& surface, uint32_t present_family, const std::vector<uint32_t>& concurrent_families);

	/**
	 * @brief Create the SwapChain and retrieve images with the given width and height. Width and height may change to meet
	 * the surface requirements.
	 *
	 * @param width Requested SwapChain width.
	 * @param height Requested SwapChain height.
	 * @param vsync Should v-sync be enabled.
	 */
	void create(uint32_t& width, uint32_t& height, bool vsync = false) const;
	
	/**
	 * @brief Acquires the next image in the swap chain.
	 * @note The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX.
	 *
	 * @param present_complete_semaphore (Optional) Semaphore that is signaled when the image is ready for use.
	 * @return Tuple containing the result of the operation and index of the next image (valid if the operation was successful)
	 */
	vk::ResultValue<uint32_t> acquireNextImage(const Semaphore&  present_complete_semaphore = {}) const;

	const std::vector<ImageView>& getImageViews() const;

	/**
	 * @brief Queue an image for presentation.
	 *
	 * @param queue Presentation queue for presenting the image.
	 * @param image_index Index of the swap chain image to queue for presentation.
	 * @param wait_semaphore (Optional) Semaphore that is waited on before the image is presented.
	 *
	 * @return Result of the queue presentation.
	 */
	vk::Result queuePresent(const Queue& queue, const Semaphore& wait_semaphore = {}) const;

	vk::Format getColorFormat() const;

protected:
	/**
	 * @brief Clean up swap chain data, surface data and disconnect from the device.
	 */
	void free() override;

private:
	struct SwapchainData {
		SwapchainData(const vk::PhysicalDevice& physical_device, const vk::Device& device, const vk::SurfaceKHR& surface,
			uint32_t present_family, std::vector<uint32_t> concurrent_families);

		vk::PhysicalDevice physical_device;
		vk::Device device;
		vk::SurfaceKHR surface;					///< Surface.
		uint32_t present_family;
		std::vector<uint32_t> concurrent_families;

		vk::SwapchainKHR swapchain{};
		vk::Format color_format{};				///< Selected swap chain color format.
		vk::ColorSpaceKHR color_space{};		///< Selected swap chain color space.

		std::vector<vk::Image> images{};		///< Swap chain images.
		std::vector<ImageView> image_views{};	///< Views of swap chain images
		uint32_t acquired_image_index{};
	};

	std::shared_ptr<SwapchainData> data_;
	std::shared_ptr<HandleManager> image_view_hm;
};

}
#endif SWAPCHAIN_H_