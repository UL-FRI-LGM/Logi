/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "base/SwapChain.h"
#include <assert.h>

namespace vkr {

SwapChain::SwapChain() : instance_(nullptr), device_(nullptr), surface_(nullptr), present_family_(nullptr),
swapchain_(nullptr), color_format_(vk::Format::eUndefined), color_space_(), image_count_(0), images_(), image_views_() {
}

void SwapChain::connect(vk::Instance& instance, VulkanDevice* device) {
	// Store instance and device pointer.
	instance_ = instance;
	device_ = device_;

	// Acquire function pointers.
	fpGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance_.getProcAddr("vkGetPhysicalDeviceSurfaceSupportKHR"));
	assert(fpGetPhysicalDeviceSurfaceSupportKHR != nullptr);
	fpGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance_.getProcAddr("vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
	assert(fpGetPhysicalDeviceSurfaceCapabilitiesKHR != nullptr);
	fpGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance_.getProcAddr("vkGetPhysicalDeviceSurfaceFormatsKHR"));
	assert(fpGetPhysicalDeviceSurfaceFormatsKHR != nullptr);
	fpGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance_.getProcAddr("vkGetPhysicalDeviceSurfacePresentModesKHR"));
	assert(fpGetPhysicalDeviceSurfacePresentModesKHR != nullptr);
	fpCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(instance_.getProcAddr("vkCreateSwapchainKHR"));
	assert(fpCreateSwapchainKHR != nullptr);
	fpDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(instance_.getProcAddr("vkDestroySwapchainKHR"));
	assert(fpDestroySwapchainKHR != nullptr);
	fpGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(instance_.getProcAddr("vkGetSwapchainImagesKHR"));
	assert(fpGetSwapchainImagesKHR != nullptr);
	fpAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(instance_.getProcAddr("vkAcquireNextImageKHR"));
	assert(fpAcquireNextImageKHR != nullptr);
	fpQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(instance_.getProcAddr("vkQueuePresentKHR"));
	assert(fpQueuePresentKHR != nullptr);
};

void SwapChain::init(vk::SurfaceKHR surface) {
	// Store surface.
	surface_ = surface;

	VkBool32 present_support = false;
	std::vector<QueueFamily*> queue_families = { device_->getGraphicalFamily(), device_->getComputeFamily(), device_->getTransferFamily() };

	for (QueueFamily* family : queue_families) {
		if (family != nullptr) {
			fpGetPhysicalDeviceSurfaceSupportKHR((VkPhysicalDevice)device_->getPhysicalDeviceHandle(), family->getFamilyIndex(), (VkSurfaceKHR) surface, &present_support);

			if (present_support) {
				present_family_ = family;
				break;
			}
		}
	}

	// Check if family with present support was found.
	if (!present_support) {
		throw std::runtime_error("Failed to find queue family with present support.");
	}

	// Get list of supported surface formats
	uint32_t format_count;
	fpGetPhysicalDeviceSurfaceFormatsKHR((VkPhysicalDevice)device_->getPhysicalDeviceHandle(), (VkSurfaceKHR)surface, &format_count, NULL);

	if (format_count == 0) {
		throw std::runtime_error("Failed to retrieve supported surface formats.");
	}

	std::vector<vk::SurfaceFormatKHR> surface_formats(format_count);
	fpGetPhysicalDeviceSurfaceFormatsKHR((VkPhysicalDevice)device_->getPhysicalDeviceHandle(), (VkSurfaceKHR)surface, &format_count, (VkSurfaceFormatKHR*)surface_formats.data());

	// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED, there is no preferred format, so we assume VK_FORMAT_B8G8R8A8_UNORM.
	if ((format_count == 1) && (surface_formats[0].format == vk::Format::eUndefined)) {
		color_format_ = vk::Format::eB8G8R8A8Unorm;
		color_space_ = vk::ColorSpaceKHR(surface_formats[0].colorSpace);
	}
	else {
		// Try to find the preferred VK_FORMAT_B8G8R8A8_UNORM format.
		bool found_B8G8R8A8_UNORM = false;
		for (auto& surface_format : surface_formats) {
			if (surface_format.format == vk::Format::eB8G8R8A8Unorm) {
				color_format_ = surface_format.format;
				color_space_ = surface_format.colorSpace;
				found_B8G8R8A8_UNORM = true;
				break;
			}
		}

		// If VK_FORMAT_B8G8R8A8_UNORM format is not available select the first available color format.
		if (!found_B8G8R8A8_UNORM) {
			color_format_ = surface_formats[0].format;
			color_space_ = surface_formats[0].colorSpace;
		}
	}
}

void SwapChain::create(uint32_t& width, uint32_t& height, bool vsync) {
	if (!device_->initialized()) {
		std::runtime_error("Tried to create SwapChain with uninitialized device.");
	}

	vk::SwapchainKHR old_swapchain = swapchain_;

	// Get physical device surface properties and formats.
	vk::SurfaceCapabilitiesKHR surface_capabilites;
	fpGetPhysicalDeviceSurfaceCapabilitiesKHR((VkPhysicalDevice)device_->getPhysicalDeviceHandle(), (VkSurfaceKHR)surface_, (VkSurfaceCapabilitiesKHR*)&surface_capabilites);

	// Get supported present modes.
	uint32_t present_mode_count;
	fpGetPhysicalDeviceSurfacePresentModesKHR((VkPhysicalDevice)device_->getPhysicalDeviceHandle(), (VkSurfaceKHR)surface_, &present_mode_count, nullptr);

	if (present_mode_count == 0) {
		throw std::runtime_error("Failed to retrieve supported present modes.");
	}

	std::vector<vk::PresentModeKHR> present_modes(present_mode_count);
	fpGetPhysicalDeviceSurfacePresentModesKHR((VkPhysicalDevice)device_->getPhysicalDeviceHandle(), (VkSurfaceKHR)surface_, &present_mode_count, (VkPresentModeKHR*)present_modes.data());


	// Create SwapChain extent.
	vk::Extent2D swapchain_extent{};
	// If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the SwapChain.
	if (surface_capabilites.currentExtent.width == (uint32_t)-1) {
		// If the surface size is undefined, the size is set to the size of the images requested.
		swapchain_extent.width = width;
		swapchain_extent.height = height;
	}
	else {
		// If the surface size is defined, the swap chain size must match
		swapchain_extent = surface_capabilites.currentExtent;
		width = surface_capabilites.currentExtent.width;
		height = surface_capabilites.currentExtent.height;
	}


	// Select a present mode for the SwapChain.
	// The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec. This mode waits for the vertical blank ("v-sync")
	vk::PresentModeKHR swapchain_present_mode = vk::PresentModeKHR::eFifo;

	// If v-sync is not requested, try to find a mailbox mode. It's the lowest latency non-tearing present mode available
	if (!vsync) {
		for (size_t i = 0; i < present_mode_count; i++) {
			if (present_modes[i] == vk::PresentModeKHR::eMailbox) {
				swapchain_present_mode = vk::PresentModeKHR::eMailbox;
				break;
			}
			else if (present_modes[i] == vk::PresentModeKHR::eImmediate) {
				swapchain_present_mode = vk::PresentModeKHR::eImmediate;
			}
		}
	}


	// Determine the number of images
	uint32_t num_swapchain_images = surface_capabilites.minImageCount + 1;
	if ((surface_capabilites.maxImageCount > 0) && (num_swapchain_images > surface_capabilites.maxImageCount)) {
		num_swapchain_images = surface_capabilites.maxImageCount;
	}

	// Find the transformation of the surface
	vk::SurfaceTransformFlagBitsKHR pre_transform;
	if (surface_capabilites.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
		// We prefer a non-rotated transform
		pre_transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
	}
	else {
		pre_transform = surface_capabilites.currentTransform;
	}


	// Find a supported composite alpha format (not all devices support alpha opaque)
	static const std::vector<vk::CompositeAlphaFlagBitsKHR> alpha_preferred = {
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
		vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
		vk::CompositeAlphaFlagBitsKHR::eInherit
	};

	vk::CompositeAlphaFlagBitsKHR composite_alpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

	// Simply select the first composite alpha format available
	for (auto& composite_alpha_flags : alpha_preferred) {
		if (surface_capabilites.supportedCompositeAlpha & composite_alpha_flags) {
			composite_alpha = composite_alpha_flags;
			break;
		};
	}


	// Swapchain creation
	vk::SwapchainCreateInfoKHR swapchain_ci = {};
	swapchain_ci.pNext = nullptr;
	swapchain_ci.surface = surface_;
	swapchain_ci.minImageCount = num_swapchain_images;
	swapchain_ci.imageFormat = color_format_;
	swapchain_ci.imageColorSpace = color_space_;
	swapchain_ci.imageExtent = { swapchain_extent.width, swapchain_extent.height };
	swapchain_ci.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	swapchain_ci.preTransform = pre_transform;
	swapchain_ci.imageArrayLayers = 1;

	if (present_family_ == device_->getGraphicalFamily()) {
		swapchain_ci.imageSharingMode = vk::SharingMode::eExclusive;
		swapchain_ci.queueFamilyIndexCount = 0;
		swapchain_ci.pQueueFamilyIndices = NULL;
	}
	else {
		uint32_t family_indices[] = { device_->getGraphicalFamily()->getFamilyIndex(), present_family_->getFamilyIndex() };
		swapchain_ci.imageSharingMode = vk::SharingMode::eConcurrent;
		swapchain_ci.queueFamilyIndexCount = 2;
		swapchain_ci.pQueueFamilyIndices = family_indices;
	}

	swapchain_ci.presentMode = swapchain_present_mode;
	swapchain_ci.oldSwapchain = old_swapchain;
	// Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
	swapchain_ci.clipped = VK_TRUE;
	swapchain_ci.compositeAlpha = composite_alpha;

	// Set additional usage flag for blitting from the swapchain images if supported
	vk::FormatProperties format_properties;
	device_->getPhysicalDeviceHandle().getFormatProperties(color_format_, &format_properties);
	if ((format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrcKHR) || (format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc)) {
		swapchain_ci.imageUsage |= vk::ImageUsageFlagBits::eTransferSrc;
	}

	// Create SwapChain.
	fpCreateSwapchainKHR((VkDevice)device_->getLogicalDeviceHandle(), (VkSwapchainCreateInfoKHR*)&swapchain_ci, nullptr, (VkSwapchainKHR*)&swapchain_);

	// If an existing swap chain is re-created, destroy the old swap chain
	// This also cleans up all the presentable images
	if (old_swapchain) {
		for (uint32_t i = 0; i < image_count_; i++) {
			device_->getLogicalDeviceHandle().destroyImageView(image_views_[i]);
		}
		fpDestroySwapchainKHR((VkDevice)device_->getLogicalDeviceHandle(), (VkSwapchainKHR)old_swapchain, nullptr);
	}
	fpGetSwapchainImagesKHR((VkDevice)device_->getLogicalDeviceHandle(), (VkSwapchainKHR)swapchain_, &image_count_, NULL);

	// Get the swap chain images
	images_.resize(image_count_);
	fpGetSwapchainImagesKHR((VkDevice)device_->getLogicalDeviceHandle(), (VkSwapchainKHR)swapchain_, &image_count_, (VkImage*)images_.data());

	// Get the swap chain buffers containing the image and imageview
	image_views_.clear();
	for (uint32_t i = 0; i < image_count_; i++) {
		vk::ImageViewCreateInfo color_attachment_view_ci{};
		color_attachment_view_ci.pNext = NULL;
		color_attachment_view_ci.format = color_format_;
		color_attachment_view_ci.components = {
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA
		};
		color_attachment_view_ci.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		color_attachment_view_ci.subresourceRange.baseMipLevel = 0;
		color_attachment_view_ci.subresourceRange.levelCount = 1;
		color_attachment_view_ci.subresourceRange.baseArrayLayer = 0;
		color_attachment_view_ci.subresourceRange.layerCount = 1;
		color_attachment_view_ci.viewType = vk::ImageViewType::e2D;
		color_attachment_view_ci.image = images_[i];

		image_views_.push_back(device_->getLogicalDeviceHandle().createImageView(color_attachment_view_ci));
	}
}

void SwapChain::cleanup() {
	if (swapchain_) {
		for (uint32_t i = 0; i < image_count_; i++) {
			device_->getLogicalDeviceHandle().destroyImageView(image_views_[i]);
		}

		// Clear images
		image_count_ = 0;
		image_views_.clear();
		images_.clear();
	}

	if (surface_) {
		fpDestroySwapchainKHR((VkDevice)device_->getLogicalDeviceHandle(), (VkSwapchainKHR)swapchain_, nullptr);
	}

	present_family_ = nullptr;
	surface_ = nullptr;
	swapchain_ = nullptr;
	device_ = nullptr;
	instance_ = nullptr;
}

std::tuple<vk::Result, uint32_t> SwapChain::acquireNextImage(vk::Semaphore present_complete_semaphore) {
	uint32_t index;
	vk::Result result = (vk::Result) fpAcquireNextImageKHR((VkDevice)device_->getLogicalDeviceHandle(), (VkSwapchainKHR)swapchain_, UINT64_MAX, (VkSemaphore)present_complete_semaphore, (VkFence)nullptr, &index);

	return std::make_tuple(result, index);
}

vk::Result SwapChain::queuePresent(vk::Queue queue, uint32_t image_index, vk::Semaphore wait_semaphore) {
	vk::PresentInfoKHR present_info{};
	present_info.pNext = NULL;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &swapchain_;
	present_info.pImageIndices = &image_index;
	// Check if a wait semaphore has been specified to wait for before presenting the image
	if (wait_semaphore) {
		present_info.pWaitSemaphores = &wait_semaphore;
		present_info.waitSemaphoreCount = 1;
	}

	return (vk::Result) fpQueuePresentKHR((VkQueue)queue, (VkPresentInfoKHR*)&present_info);
}

SwapChain::~SwapChain() {
	cleanup();
}

}