#include <vk_device.h>
#include <string>
#include <set>
#include <vector>

namespace vkr {

VulkanDevice::VulkanDevice(VkPhysicalDevice device, uint32_t device_idx)
	: physical_device_(device), logical_device_(nullptr), device_idx_(device_idx), graphics_family_idx_(UINT_MAX) {
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
	for (size_t i = 0; i < queue_family_properties.size(); i++) {
		if (queue_family_properties[i].queueCount > 0 && queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphics_capabilities_ = true;

			// Add new queue entry.
			graphics_queue_families_.push_back(QueueInfo());
			QueueInfo &queue_info = graphics_queue_families_.back();
			queue_info.family_idx = i;
			queue_info.queues.resize(queue_family_properties[i].queueCount, nullptr);
		}
	}

	// Fetch available extensions.
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	available_extensions_.resize(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions_.data());
}

bool VulkanDevice::supportsExtensions(const std::vector<const char*> &extensions) {
	std::set<std::string> extensions_set(extensions.begin(), extensions.end());

	// Cross the queried extensions off the list.
	for (const auto& extension : available_extensions_) {
		extensions_set.erase(extension.extensionName);
	}

	return extensions_set.empty();
}

bool VulkanDevice::initialize(const std::vector<const char*> &requested_extensions) {
	// Check if the requested extensions are supported.
	// NOTE: We currently support only devices with graphics_capabilities
	if (!supportsExtensions(requested_extensions) && hasGraphicsCapabilities()) {
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
	for (QueueInfo &queueFamily : graphics_queue_families_) {

		VkDeviceQueueCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		create_info.pNext = nullptr;
		create_info.queueFamilyIndex = queueFamily.family_idx;
		create_info.queueCount = queueFamily.queues.size();
		create_info.pQueuePriorities = &queue_priority;

		queue_create_infos.push_back(create_info);
	}

	// Construct create info for logical device
	// Create logical device
	const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	uint32_t numExtensions = sizeof(extensions) / sizeof(extensions[0]);

	VkDeviceCreateInfo device_info;
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = nullptr;
	device_info.flags = 0;
	device_info.queueCreateInfoCount = (uint32_t) queue_create_infos.size();
	device_info.pQueueCreateInfos = queue_create_infos.data();
	device_info.pEnabledFeatures = &mDeviceFeatures;
	device_info.enabledExtensionCount = requested_extensions.size();
	device_info.ppEnabledExtensionNames = requested_extensions.data();
	device_info.enabledLayerCount = 0;
	device_info.ppEnabledLayerNames = nullptr;

	// TODO


}

}