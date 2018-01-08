/*
* Vulkan Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include <vulkan/vulkan.hpp>
#include "base/VulkanDevice.h"

namespace vkr {


VulkanDevice::VulkanDevice(vk::PhysicalDevice& device)
	: physical_device_(device), logical_device_(nullptr), device_properties_(), device_features_(), memory_properties_(),
	available_extensions_(), enabled_features_(), queue_family_properties_(), queue_families_(), pipeline_resources_(nullptr), initialized_(false) {

	physical_device_ = device;

	// Fetch device meta data.
	physical_device_.getProperties(&device_properties_);
	physical_device_.getFeatures(&device_features_);
	physical_device_.getMemoryProperties(&memory_properties_);
	
	// Fetch available extensions.
	uint32_t extension_count;
	physical_device_.enumerateDeviceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<vk::ExtensionProperties> extensions(extension_count);
	physical_device_.enumerateDeviceExtensionProperties(nullptr, &extension_count, extensions.data());

	// Store names of the extensions into a set.
	std::for_each(extensions.begin(), extensions.end(), [this](vk::ExtensionProperties& extension) {
		available_extensions_.insert(extension.extensionName);
	});

	// Fetch queue families.
	uint32_t queue_family_count;
	physical_device_.getQueueFamilyProperties(&queue_family_count, nullptr);

	queue_family_properties_.resize(queue_family_count);
	physical_device_.getQueueFamilyProperties(&queue_family_count, queue_family_properties_.data());

	// Try to find dedicated graphic, compute and transfer queues.
	for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties_.size()); i++) {
		if (queue_families_[static_cast<size_t>(QueueFamilyType::GRAPHIC)].family_index == UINT_MAX && queue_family_properties_[i].queueFlags & vk::QueueFlagBits::eGraphics) {
			queue_families_[static_cast<size_t>(QueueFamilyType::GRAPHIC)].family_index = i;
		}
		else if (queue_families_[static_cast<size_t>(QueueFamilyType::COMPUTE)].family_index == UINT_MAX && queue_family_properties_[i].queueFlags & vk::QueueFlagBits::eCompute) {
			queue_families_[static_cast<size_t>(QueueFamilyType::COMPUTE)].family_index = i;
		}
		else if (queue_families_[static_cast<size_t>(QueueFamilyType::TRANSFER)].family_index == UINT_MAX && queue_family_properties_[i].queueFlags & vk::QueueFlagBits::eTransfer) {
			queue_families_[static_cast<size_t>(QueueFamilyType::TRANSFER)].family_index = i;
		}
	}
}

std::vector<vk::DeviceQueueCreateInfo> VulkanDevice::generateQueueCreateInfos(const QueueConfig& queue_config) {
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};

	// Populate create info structure for graphic queues.
	if (queue_config.graphic_count > 0) {
		uint32_t family_index = queue_families_[static_cast<size_t>(QueueFamilyType::GRAPHIC)].family_index;

		// Make sure that device supports has graphic queue family and enough graphic queues available.
		if (family_index == UINT32_MAX || queue_family_properties_[family_index].queueCount < queue_config.graphic_count) {
			std::runtime_error("Device (" + std::string(device_properties_.deviceName) + ") does not provide enough graphic queues.");
		}

		// Create QueueCreateInfo.
		vk::DeviceQueueCreateInfo queueInfo{};
		queueInfo.queueFamilyIndex = family_index;
		queueInfo.queueCount = queue_config.graphic_count;
		queueInfo.pQueuePriorities = &kDefaultQueuePriority;
		queue_create_infos.push_back(queueInfo);

		// Store queue count.
		queue_families_[static_cast<size_t>(QueueFamilyType::GRAPHIC)].queue_count = queue_config.graphic_count;
	}

	// Populate create info structure for compute queues.
	if (queue_config.compute_count > 0) {
		uint32_t family_index = queue_families_[static_cast<size_t>(QueueFamilyType::COMPUTE)].family_index;

		// Make sure that device supports has graphic queue family and enough graphic queues available.
		if (family_index == UINT32_MAX || queue_family_properties_[family_index].queueCount < queue_config.compute_count) {
			std::runtime_error("Device (" + std::string(device_properties_.deviceName) + ") does not provide enough compute queues.");
		}

		// Create QueueCreateInfo.
		vk::DeviceQueueCreateInfo queueInfo{};
		queueInfo.queueFamilyIndex = family_index;
		queueInfo.queueCount = queue_config.compute_count;
		queueInfo.pQueuePriorities = &kDefaultQueuePriority;
		queue_create_infos.push_back(queueInfo);

		// Store queue count.
		queue_families_[static_cast<size_t>(QueueFamilyType::COMPUTE)].queue_count = queue_config.compute_count;
	}

	// Populate create info structure for a dedicated transfer queue.
	if (queue_config.dedicated_transfer) {
		uint32_t family_index = queue_families_[static_cast<size_t>(QueueFamilyType::TRANSFER)].family_index;

		if (family_index == UINT32_MAX) {
			std::runtime_error("Device (" + std::string(device_properties_.deviceName) + ") does not provide dedicated transfer queue.");
		}

		// Create QueueCreateInfo.
		vk::DeviceQueueCreateInfo queueInfo{};
		queueInfo.queueFamilyIndex = family_index;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &kDefaultQueuePriority;
		queue_create_infos.push_back(queueInfo);

		queue_families_[static_cast<size_t>(QueueFamilyType::TRANSFER)].queue_count = 1;
	}

	return queue_create_infos;
}

vk::CommandPool VulkanDevice::createCommandPool(const uint32_t family_index, const vk::CommandPoolCreateFlags flags) {
	// Populate create info.
	vk::CommandPoolCreateInfo cmd_pool_info{};
	cmd_pool_info.queueFamilyIndex = family_index;
	cmd_pool_info.flags = flags;

	// Create command pool.
	vk::CommandPool cmd_pool;
	vk::Result result = logical_device_.createCommandPool(&cmd_pool_info, nullptr, &cmd_pool);

	if (result != vk::Result::eSuccess) {
		throw std::runtime_error("Failed to create command pool for device (" + std::string(device_properties_.deviceName) + ").");
	}

	return cmd_pool;
}

void VulkanDevice::initialize(const vk::PhysicalDeviceFeatures& features, const std::vector<char*>& extensions, const QueueConfig& queue_config) {

	// Generate queue create infos.
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos = generateQueueCreateInfos(queue_config);

	// Construct device create info.
	vk::DeviceCreateInfo device_create_info{};
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());;
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.pEnabledFeatures = &features;

	if (extensions.size() > 0) {
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		device_create_info.ppEnabledExtensionNames = extensions.data();
	}

	// Try to create logical device.
	vk::Result result = physical_device_.createDevice(&device_create_info, nullptr, &logical_device_);

	// Check if the logical device was successfully created.
	if (result != vk::Result::eSuccess) {
		throw std::runtime_error("Failed to create logical device.");
	}

	// Store list of enabled features.
	enabled_features_ = features;

	// Initialize command pools and fetch queues for each queue family.
	for (QueueFamily& queue_family : queue_families_) {
		// Fetch queues.
		for (uint32_t i = 0; i < queue_family.queue_count; i++) {
			queue_family.queues.push_back(logical_device_.getQueue(queue_family.family_index, i));
		}

		// Create command pool. (do not bother if there is no queue to use it)
		if (queue_family.family_index != UINT32_MAX && queue_family.queue_count > 0) {
			queue_family.command_pool = createCommandPool(queue_family.family_index);
		}
	}

	pipeline_resources_ = std::make_unique<PipelineResources>(logical_device_);

	// Mark the device as initialized.
	initialized_ = true;
}

bool VulkanDevice::extensionSupported(const std::string& extension_name) const {
	return available_extensions_.find(extension_name) != available_extensions_.end();
}

bool VulkanDevice::supportsQueueFamily(const QueueFamilyType type) const {
	return queue_families_[static_cast<size_t>(type)].family_index != UINT32_MAX;
}

uint32_t VulkanDevice::getQueueCount(const QueueFamilyType type) const {
	return queue_families_[static_cast<size_t>(type)].queue_count;
}

const std::vector<vk::QueueFamilyProperties>& VulkanDevice::getQueueFamilyProperties() const {
	return queue_family_properties_;
}

vk::PhysicalDevice VulkanDevice::getPhysicalDeviceHandle() const {
	return physical_device_;
}

vk::Device VulkanDevice::getLogicalDeviceHandle() const {
	return logical_device_;
}

uint32_t VulkanDevice::getFamilyIndex(QueueFamilyType type) const {
	return queue_families_[static_cast<size_t>(type)].family_index;
}

const vk::PhysicalDeviceProperties& VulkanDevice::properties() const {
	return device_properties_;
}

const vk::PhysicalDeviceFeatures& VulkanDevice::features() const {
	return device_features_;
}

const vk::PhysicalDeviceMemoryProperties& VulkanDevice::memoryProperties() const {
	return memory_properties_;
}

void VulkanDevice::createDescriptorPool(const DescriptorsCount& pool_sizes, bool releasable_sets) {
	if (!initialized_) {
		throw std::runtime_error("Tried to create descriptor pool on an uninitialized device.");
	}

	descriptor_pool_ = std::make_unique<DescriptorPool>(logical_device_, pool_sizes, releasable_sets);
}

DescriptorPool* VulkanDevice::getDescriptorPool() {
	return descriptor_pool_.get();
}

bool VulkanDevice::initialized() const {
	return initialized_;
}

PipelineResources* VulkanDevice::getPipelineResources() {
	return pipeline_resources_.get();
}

VulkanDevice::~VulkanDevice() {
	// If logical device was created, destroy it.
	if (logical_device_) {
		descriptor_pool_.reset();
		pipeline_resources_.reset();

		// Destroy command pools
		for (QueueFamily& queue_family : queue_families_) {
			if (queue_family.family_index != UINT32_MAX && queue_family.queue_count > 0) {
				logical_device_.destroyCommandPool(queue_family.command_pool);
			}
		}

		logical_device_.destroy();
	}
}

} /// !namespace vkr