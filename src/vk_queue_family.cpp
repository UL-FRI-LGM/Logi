#include "vk_queue_family.h"
#include "vk_device.h"
#include "vk_queue.h"

#include <vulkan/vulkan.hpp>
#include <vector>

namespace vkr {

VulkanQueueFamily::VulkanQueueFamily(VulkanDevice &owner_device, uint32_t family_idx, VkQueueFamilyProperties properties) 
	: owner_device_(owner_device), family_idx_(family_idx), properties_(properties) {

	queues_.resize(properties_.queueCount, nullptr);
}

void VulkanQueueFamily::initializeQueues() {
	// Create queues.
	for (uint32_t i = 0; i < properties_.queueCount; i++) {
		VkQueue queue;
		vkGetDeviceQueue(owner_device_.getLogicalDeviceHandle(), family_idx_, i, &queue);

		queues_[i] = std::make_shared<VulkanQueue>(*this, queue);
	}
}

void VulkanQueueFamily::initializeCommandPool(VkCommandPoolCreateFlags flags) {
	VkCommandPoolCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	create_info.queueFamilyIndex = family_idx_;
	create_info.flags = flags;

	if (vkCreateCommandPool(owner_device_.getLogicalDeviceHandle(), &create_info, nullptr, &command_pool_) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool!");
	}
}

bool VulkanQueueFamily::presentationSupport(VkSurfaceKHR surface) const {
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(owner_device_.getPhysicalDeviceHandle(), family_idx_, surface, &presentSupport);

	return static_cast<bool>(presentSupport);
}

uint32_t VulkanQueueFamily::getFamilyIdx() const {
	return family_idx_;
}


uint32_t VulkanQueueFamily::getQueueCount() const {
	return properties_.queueCount;
}



}