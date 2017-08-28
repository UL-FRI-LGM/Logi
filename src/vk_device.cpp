#include <vk_device.h>
#include <string>
#include <set>
#include <vector>

namespace vkr {

VulkanDevice::VulkanDevice(VkPhysicalDevice device)
	: physical_device_(device), logical_device_(nullptr), initialized_(false){
	this->physical_device_ = device;

	// Get device meta data.
	vkGetPhysicalDeviceProperties(device, &device_properties_);
	vkGetPhysicalDeviceFeatures(device, &device_features_);
	vkGetPhysicalDeviceMemoryProperties(device, &memory_properties_);

	// Get queue family count.
	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

	// Get queue family properties.
	std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_properties.data());

	// Check if the device has graphics capabilities.
	for (uint32_t i = 0u; i < queue_family_properties.size(); i++) {
		// Disregard queue families with 0 queues.
		if (queue_family_properties[i].queueCount == 0u) {
			continue;
		}

		std::shared_ptr<VulkanQueueFamily> queue_family = std::make_shared<VulkanQueueFamily>(*this, i, queue_family_properties[i]);

		// Sort queue families in queue families with graphical and compute only capabilities
		if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphics_queue_families_.push_back(queue_family);
		}
		else if (queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			compute_queue_families_.push_back(queue_family);
		}
		else if (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			transfer_queue_families_.push_back(queue_family);
		}
	}

	// Fetch available extensions.
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	available_extensions_.resize(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions_.data());
}

VulkanDevice::~VulkanDevice() {
	if (initialized_) {
		waitIdle();
	}

	// Clear the queues.
	graphics_queue_families_.clear();
	compute_queue_families_.clear();
	transfer_queue_families_.clear();

	// Destroy the logical device.
	if (initialized_) {
		vkDestroyDevice(logical_device_, nullptr);
	}
}

bool VulkanDevice::supportsExtensions(const std::vector<const char*> &extensions) const {
	std::set<std::string> extensions_set(extensions.begin(), extensions.end());

	// Cross the queried extensions off the list.
	for (const auto& extension : available_extensions_) {
		extensions_set.erase(extension.extensionName);
	}

	return extensions_set.empty();
}


bool VulkanDevice::initializeLogicalDevice(const std::vector<const char*> &requested_extensions, const VkPhysicalDeviceFeatures *requested_features) {
	// Check if the requested extensions are supported.
	// NOTE: Device must have at least compute capabilities.
	if (!supportsExtensions(requested_extensions) && hasComputeCapabilities()) {
		return false;
	}

	// Get queue family count.
	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);

	// Get queue family properties.
	std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_family_properties.data());

	// Construct create infos for queues.
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	float queue_priority = 1.0f;

	auto addQueueCreateInfo = [&] (std::shared_ptr<VulkanQueueFamily> &queue_family) {
		VkDeviceQueueCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		create_info.pNext = nullptr;
		create_info.queueFamilyIndex = queue_family->getFamilyIdx();
		create_info.queueCount = queue_family->getQueueCount();
		create_info.pQueuePriorities = &queue_priority;

		queue_create_infos.push_back(create_info);
	};

	// Build create info structures for both graphics and compute queue families.
	std::for_each(graphics_queue_families_.begin(), graphics_queue_families_.end(), addQueueCreateInfo);
	std::for_each(compute_queue_families_.begin(), compute_queue_families_.end(), addQueueCreateInfo);
	std::for_each(transfer_queue_families_.begin(), transfer_queue_families_.end(), addQueueCreateInfo);

	// Construct logical device
	VkDeviceCreateInfo device_info;
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = nullptr;
	device_info.flags = 0;
	device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	device_info.pQueueCreateInfos = queue_create_infos.data();
	device_info.pEnabledFeatures = requested_features;
	device_info.enabledExtensionCount = static_cast<uint32_t>(requested_extensions.size());
	device_info.ppEnabledExtensionNames = requested_extensions.data();
	// Note: Device validation layers are deprecated.
	device_info.enabledLayerCount = 0u;
	device_info.ppEnabledLayerNames = nullptr;

	// Try to create logical device.
	if (vkCreateDevice(physical_device_, &device_info, nullptr, &logical_device_) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device!");
	}

	// Initialize queues.
	auto queueFamilyInit = [] (std::shared_ptr<VulkanQueueFamily> &queue_family) { queue_family->initializeQueues(); };

	std::for_each(graphics_queue_families_.begin(), graphics_queue_families_.end(), queueFamilyInit);
	std::for_each(compute_queue_families_.begin(), compute_queue_families_.end(), queueFamilyInit);
	std::for_each(transfer_queue_families_.begin(), transfer_queue_families_.end(), queueFamilyInit);

	return true;
}

void VulkanDevice::waitIdle() const {
	VkResult result = vkDeviceWaitIdle(logical_device_);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Device wait idle error!");
	}
}

bool VulkanDevice::hasGraphicsCapabilities() const {
	return graphics_queue_families_.size() > 0;
}

bool VulkanDevice::hasComputeCapabilities() const {
	return (compute_queue_families_.size() > 0) || (graphics_queue_families_.size() > 0);
}

VkPhysicalDeviceFeatures VulkanDevice::getSupportedFeatures() const {
	return device_features_;
};

VkPhysicalDevice VulkanDevice::getPhysicalDeviceHandle() {
	return physical_device_;
}

VkDevice VulkanDevice::getLogicalDeviceHandle() {
	return logical_device_;
}

}