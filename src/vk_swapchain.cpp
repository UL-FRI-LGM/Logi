#include "vk_swapchain.h"
#include <vulkan/vulkan.hpp>
#include "vk_device.h"

namespace vkr {

VulkanSwapChain& VulkanSwapChain::getInstance() {
	// Instantiated on first use
	static VulkanSwapChain instance;

	return instance;
}

void VulkanSwapChain::initialize(VulkanDevice *device, VkSurfaceKHR surface, uint32_t width, uint32_t height, bool vsync, VkFormat color_format, VkFormat depth_format) {
	if (initialized_) {
		throw std::runtime_error("Tried to reinitialize swap-chain without cleaning up.");
	}

	device_ = device;

	// Fetch support details.
	SwapChainSupportDetails support_details = querySwapChainSupport(device_->getPhysicalDeviceHandle(), surface);

	if (support_details.formats.empty() || support_details.present_modes.empty()) {
		throw std::runtime_error("Given device cannot present on the specified surface.");
	}

	// Set the extent based on the devices capabilities.
	VkExtent2D extent = {
		std::max(support_details.capabilities.minImageExtent.width, std::min(support_details.capabilities.maxImageExtent.width, width)),
		std::max(support_details.capabilities.minImageExtent.height, std::min(support_details.capabilities.maxImageExtent.height, height))
	};

	// Pick presentation mode and surface format.
	VkPresentModeKHR presentation_mode = pickSwapchainPresentMode(support_details.present_modes, vsync);
	VkSurfaceFormatKHR surface_format = pickSwapchainSurfaceFormat(support_details.formats);

	// Try to acquire one more image than the minimum so that the triple buffering can be properly implemented.
	uint32_t image_count = support_details.capabilities.minImageCount + 1;
	if (support_details.capabilities.maxImageCount > 0 && image_count > support_details.capabilities.maxImageCount) {
		image_count = support_details.capabilities.maxImageCount;
	}

	// Create swap-chain.
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = image_count;
	createInfo.imageFormat = surface_format.format;
	createInfo.imageColorSpace = surface_format.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createInfo.preTransform = support_details.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentation_mode;
	createInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(device_->getLogicalDeviceHandle(), &createInfo, nullptr, &swapchain_) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap-chain!");
	}

	// Retrieve VkImages.
	std::vector<VkImage> swapchain_images;

	vkGetSwapchainImagesKHR(device_->getLogicalDeviceHandle(), swapchain_, &image_count, nullptr);
	swapchain_images.resize(image_count);
	vkGetSwapchainImagesKHR(device_->getLogicalDeviceHandle(), swapchain_, &image_count, swapchain_images.data());

	for (auto &image : swapchain_images) {
		textures_.push_back(std::make_unique<VulkanTexture>(image, device_, false));
	}
}

VkPresentModeKHR VulkanSwapChain::pickSwapchainPresentMode(const std::vector<VkPresentModeKHR> &supported_present_modes, bool vsync) {
	// Default presentation mode with v-sync enabled. 
	// Note: Vulkan requires this mode to be supported.
	VkPresentModeKHR selected_mode = VK_PRESENT_MODE_FIFO_KHR;

	// If vertical synchronization is disabled try to either pick VK_PRESENT_MODE_IMMEDIATE_KHR or VK_PRESENT_MODE_FIFO_RELAXED_KHR.
	if (!vsync) {
		for (const VkPresentModeKHR &mode : supported_present_modes) {
			if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				selected_mode = mode;
				break;
			}

			if (mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
				selected_mode = mode;
		}
	}

	return selected_mode;
}

VkSurfaceFormatKHR VulkanSwapChain::pickSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &supported_surface_formats) {
	if (supported_surface_formats.size() == 1 && supported_surface_formats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& surface_format : supported_surface_formats) {
		if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return surface_format;
		}
	}

	return supported_surface_formats[0];
}

VulkanSwapChain::SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails details;

	// Fetch surface capabilities.
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Fetch supported formats.
	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

	if (format_count != 0) {
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
	}

	// Fetch supported presentation modes.
	uint32_t presentation_modes_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentation_modes_count, nullptr);

	if (presentation_modes_count != 0) {
		details.present_modes.resize(presentation_modes_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentation_modes_count, details.present_modes.data());
	}

	return details;
}

}