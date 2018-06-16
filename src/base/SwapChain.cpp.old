/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "base/SwapChain.h"
#include <assert.h>

namespace logi {

SwapChain::SwapChain(std::weak_ptr<HandleManager>& owner, const vk::Device& device, vk::SurfaceKHR& surface, uint32_t present_family, const std::vector<uint32_t>& concurrent_families)
	: DependentDestroyableHandle(owner), device_(device){
	// Store instance and device pointer.
	instance_ = instance;
	device_ = device;
};

void SwapChain::init(vk::SurfaceKHR surface) {
	// Store surface.
	surface_ = surface;

	VkBool32 present_support = false;
	std::vector<QueueFamily*> queue_families = { device_->getGraphicalFamily(), device_->getComputeFamily(), device_->getTransferFamily() };

	for (QueueFamily* family : queue_families) {
		if (family != nullptr) {
			device_->getPhysicalDeviceHandle().getSurfaceSupportKHR(family->getFamilyIndex(), surface, &present_support);

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
	std::vector<vk::SurfaceFormatKHR> surface_formats = device_->getPhysicalDeviceHandle().getSurfaceFormatsKHR(surface_);

	if (surface_formats.empty()) {
		throw std::runtime_error("Failed to retrieve supported surface formats.");
	}

	// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED, there is no preferred format, so we assume VK_FORMAT_B8G8R8A8_UNORM.
	if ((surface_formats.size() == 1) && (surface_formats[0].format == vk::Format::eUndefined)) {
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

	vk::PhysicalDevice physical_device = device_->getPhysicalDeviceHandle();
	vk::Device logical_device = device_->getLogicalDeviceHandle();
	vk::SwapchainKHR old_swapchain = swapchain_;

	// Get physical device surface properties and formats.
	vk::SurfaceCapabilitiesKHR surface_capabilites = physical_device.getSurfaceCapabilitiesKHR(surface_);

	// Get supported present modes.
	std::vector<vk::PresentModeKHR> present_modes = physical_device.getSurfacePresentModesKHR(surface_);

	if (present_modes.empty()) {
		throw std::runtime_error("Failed to retrieve supported present modes.");
	}

	// Create SwapChain extent.
	vk::Extent2D swapchain_extent{};
	// If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the SwapChain.
	if (surface_capabilites.currentExtent.width == static_cast<uint32_t>(-1)) {
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
		for (size_t i = 0; i < present_modes.size(); i++) {
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
	swapchain_ci.imageExtent = vk::Extent2D(swapchain_extent.width, swapchain_extent.height);
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
	physical_device.getFormatProperties(color_format_, &format_properties);

	if ((format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrcKHR) || (format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc)) {
		swapchain_ci.imageUsage |= vk::ImageUsageFlagBits::eTransferSrc;
	}

	// Create SwapChain.
	swapchain_ = logical_device.createSwapchainKHR(swapchain_ci);

	// If an existing swap chain is re-created, destroy the old swap chain
	// This also cleans up all the presentable images
	if (old_swapchain) {
		for (uint32_t i = 0; i < images_.size(); i++) {
			device_->getLogicalDeviceHandle().destroyImageView(image_views_[i]);
		}
		logical_device.destroySwapchainKHR(old_swapchain);
	}

	images_ = logical_device.getSwapchainImagesKHR(swapchain_);

	// Get the swap chain buffers containing the image and imageview
	image_views_.clear();
	for (uint32_t i = 0; i < images_.size(); i++) {
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
		for (uint32_t i = 0; i < images_.size(); i++) {
			device_->getLogicalDeviceHandle().destroyImageView(image_views_[i]);
		}

		// Clear images
		image_views_.clear();
		images_.clear();
	}

	if (surface_) {
		device_->getLogicalDeviceHandle().destroySwapchainKHR(swapchain_);
	}

	present_family_ = nullptr;
	surface_ = nullptr;
	swapchain_ = nullptr;
	device_ = nullptr;
}

vk::ResultValue<uint32_t> SwapChain::acquireNextImage(vk::Semaphore present_complete_semaphore) {
	device_->getLogicalDeviceHandle().acquireNextImageKHR(swapchain_, UINT64_MAX, present_complete_semaphore, nullptr);

	return device_->getLogicalDeviceHandle().acquireNextImageKHR(swapchain_, UINT64_MAX, present_complete_semaphore, nullptr);
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

	return queue.presentKHR(present_info);
}

SwapChain::~SwapChain() {
}

}
