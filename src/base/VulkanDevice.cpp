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

QueueConfig::QueueConfig(uint32_t graphic_count, uint32_t compute_count, uint32_t transfer_count) : graphic_count(graphic_count), compute_count(compute_count), transfer_count(transfer_count) {
}

#pragma region INITIALIZATION
VulkanDevice::VulkanDevice(vk::PhysicalDevice& device)
	: physical_device_(device), logical_device_(nullptr), device_properties_(), device_features_(), memory_properties_(),
	available_extensions_(), enabled_features_(), graphical_family_(nullptr), compute_family_(nullptr), transfer_family_(nullptr), program_manager_(nullptr), descriptor_pool_(nullptr), allocation_manager_(nullptr), initialized_(false) {

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

	// Try to find dedicated graphic, compute and transfer queues.
	std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device_.getQueueFamilyProperties();

	for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++) {

		if (queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) {
			if (graphical_family_.get() == nullptr) {
				graphical_family_ = std::make_unique<QueueFamily>(i, queue_family_properties[i].queueFlags, queue_family_properties[i].queueCount, queue_family_properties[i].timestampValidBits, queue_family_properties[i].minImageTransferGranularity);
			}
		}
		else if (queue_family_properties[i].queueFlags & vk::QueueFlagBits::eCompute) {
			if (compute_family_.get() == nullptr) {
				compute_family_ = std::make_unique<QueueFamily>(i, queue_family_properties[i].queueFlags, queue_family_properties[i].queueCount, queue_family_properties[i].timestampValidBits, queue_family_properties[i].minImageTransferGranularity);
			}
		}
		else if (queue_family_properties[i].queueFlags & vk::QueueFlagBits::eTransfer) {
			if (transfer_family_.get() == nullptr) {
				transfer_family_ = std::make_unique<QueueFamily>(i, queue_family_properties[i].queueFlags, queue_family_properties[i].queueCount, queue_family_properties[i].timestampValidBits, queue_family_properties[i].minImageTransferGranularity);
			}
		}
	}
}

void VulkanDevice::initialize(const vk::PhysicalDeviceFeatures& features, const std::vector<char*>& extensions, const QueueConfig& queue_config) {

	// Generate queue create infos.
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
	std::vector<float> priorities(std::max(std::max(queue_config.graphic_count, queue_config.compute_count), queue_config.transfer_count), 0);

	// Populate create info structure for graphic queues.
	if (queue_config.graphic_count > 0) {
		// Make sure that device supports has graphic queue family and enough graphic queues available.
		if (graphical_family_.get() == nullptr || graphical_family_->getMaxSupportedQueueCount() < queue_config.graphic_count) {
			throw std::runtime_error("Device (" + std::string(device_properties_.deviceName) + ") does not provide enough graphic queues.");
		}

		// Create QueueCreateInfo.
		vk::DeviceQueueCreateInfo queueInfo{};
		queueInfo.queueFamilyIndex = graphical_family_->getFamilyIndex();
		queueInfo.queueCount = queue_config.graphic_count;
		queueInfo.pQueuePriorities = priorities.data();

		queue_create_infos.push_back(queueInfo);
	}

	// Populate create info structure for compute queues.
	if (queue_config.compute_count > 0) {
		// Make sure that device supports has compute queue family and enough compute queues available.
		if (compute_family_.get() == nullptr || compute_family_->getMaxSupportedQueueCount() < queue_config.compute_count) {
			throw std::runtime_error("Device (" + std::string(device_properties_.deviceName) + ") does not provide enough compute queues.");
		}

		// Create QueueCreateInfo.
		vk::DeviceQueueCreateInfo queueInfo{};
		queueInfo.queueFamilyIndex = compute_family_->getFamilyIndex();
		queueInfo.queueCount = queue_config.compute_count;
		queueInfo.pQueuePriorities = priorities.data();

		queue_create_infos.push_back(queueInfo);
	}

	// Populate create info structure for a dedicated transfer queue.
	if (queue_config.transfer_count > 0) {
		// Make sure that device supports has transfer queue family and enough transfer queues available.
		if (transfer_family_.get() == nullptr || transfer_family_->getMaxSupportedQueueCount() < queue_config.transfer_count) {
			throw std::runtime_error("Device (" + std::string(device_properties_.deviceName) + ") does not provide enough transfer queues.");
		}

		// Create QueueCreateInfo.
		vk::DeviceQueueCreateInfo queueInfo{};
		queueInfo.queueFamilyIndex = transfer_family_->getFamilyIndex();
		queueInfo.queueCount = queue_config.transfer_count;
		queueInfo.pQueuePriorities = priorities.data();

		queue_create_infos.push_back(queueInfo);
	}

	// Construct device create info.
	vk::DeviceCreateInfo device_create_info{};
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());;
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.pEnabledFeatures = &features;

	// Store list of enabled features.
	enabled_features_ = features;

	if (extensions.size() > 0) {
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		device_create_info.ppEnabledExtensionNames = extensions.data();
	}

	// Create logical device.
	logical_device_ = physical_device_.createDevice(device_create_info);

	// Initialize queues.
    if (graphical_family_) {
        graphical_family_->initialize(logical_device_, queue_config.graphic_count);
    }
    if (compute_family_) {
        compute_family_->initialize(logical_device_, queue_config.compute_count);
    }
    if (transfer_family_) {
        transfer_family_->initialize(logical_device_, queue_config.transfer_count);
    }

	// Create allocation and program manager
	allocation_manager_ = std::make_unique<AllocationManager>(physical_device_, logical_device_);
	program_manager_ = std::make_unique<ProgramManager>(logical_device_);

	// Mark the device as initialized.
	initialized_ = true;
}
#pragma endregion

#pragma region META DATA GETTERS
bool VulkanDevice::extensionSupported(const std::string& extension_name) const {
	return available_extensions_.find(extension_name) != available_extensions_.end();
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

uint32_t VulkanDevice::getGraphicsQueueCount() const {
	if (graphical_family_.get() != nullptr) {
		return graphical_family_->getMaxSupportedQueueCount();
	}
	else {
		return 0;
	}
}

uint32_t VulkanDevice::getComputeQueueCount() const {
	if (compute_family_.get() != nullptr) {
		return compute_family_->getMaxSupportedQueueCount();
	}
	else {
		return 0;
	}
}

uint32_t VulkanDevice::getTransferQueueCount() const {
	if (transfer_family_.get() != nullptr) {
		return transfer_family_->getMaxSupportedQueueCount();
	}
	else {
		return 0;
	}
}
#pragma endregion

#pragma region QUEUE FAMILIES
QueueFamily* VulkanDevice::getGraphicalFamily() {
	return graphical_family_.get();
}

QueueFamily* VulkanDevice::getComputeFamily() {
	return compute_family_.get();
}

QueueFamily* VulkanDevice::getTransferFamily() {
	return transfer_family_.get();
}
#pragma endregion


void VulkanDevice::createDescriptorPool(const DescriptorsCount& pool_sizes, bool releasable_sets) {
	if (!initialized_) {
		throw std::runtime_error("Tried to create descriptor pool on an uninitialized device.");
	}

	descriptor_pool_ = std::make_unique<DescriptorPool>(logical_device_, pool_sizes, releasable_sets);
}

DescriptorPool* VulkanDevice::getDescriptorPool() {
	return descriptor_pool_.get();
}

ProgramManager* VulkanDevice::getProgramManager() {
	return program_manager_.get();
}

AllocationManager* VulkanDevice::getAllocationManager() {
	return allocation_manager_.get();
}

void VulkanDevice::executeDescriptorsUpdate(const DescriptorsUpdate& update) const {
	logical_device_.updateDescriptorSets(update.getWriteOperations(), update.getCopyOperations());
}

bool VulkanDevice::initialized() const {
	return initialized_;
}

#pragma region DEVICE HANDLE GETTERS
const vk::PhysicalDevice& VulkanDevice::getPhysicalDeviceHandle() const {
	return physical_device_;
}

const vk::Device& VulkanDevice::getLogicalDeviceHandle() const {
	return logical_device_;
}
#pragma endregion

VulkanDevice::~VulkanDevice() {
	// If logical device was created, destroy it.
	if (logical_device_) {
		allocation_manager_.reset();
		descriptor_pool_.reset();
		program_manager_.reset();

		// Destroy command pools
		graphical_family_.reset();
		compute_family_.reset();
		transfer_family_.reset();

		// Finally destroy the logical device
		logical_device_.destroy();
	}
}

} /// !namespace vkr
