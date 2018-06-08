/*
* Vulkan Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include <vulkan/vulkan.hpp>
#include "base/VulkanDevice.h"

namespace logi {

#pragma region INITIALIZATION
VulkanDevice::VulkanDevice() : DependentDestroyableHandle({}, false), data_(nullptr), handle_manager_(nullptr) {}

VulkanDevice::VulkanDevice(const std::weak_ptr<HandleManager>& owner, vk::PhysicalDevice& device) 
	: DependentDestroyableHandle(owner), data_(std::make_shared<DeviceData>(device)), handle_manager_(std::make_shared<HandleManager>()) {}

void VulkanDevice::initialize(const vk::PhysicalDeviceFeatures& features, const std::vector<const char*>& extensions, const std::vector<QueueFamilyProperties>& queue_family_configs) const {
	// Generate queue create infos.
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
	queue_create_infos.reserve(queue_family_configs.size());

	for (const QueueFamilyProperties& config : queue_family_configs) {
		// Ignore configurations with 0 queue count.
		if (config.queue_count > 0) {
			vk::DeviceQueueCreateInfo queue_ci{};
			queue_ci.flags = config.create_flags;
			queue_ci.queueCount = config.queue_count;
			queue_ci.pQueuePriorities = config.queue_priorities.data();

			queue_create_infos.emplace_back(queue_ci);
		}
	}

	// Construct device create info.
	vk::DeviceCreateInfo device_create_info{};
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());;
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.pEnabledFeatures = &features;

	// Store list of enabled features.
	data_->enabled_features = features;
	data_->enabled_extensions = extensions;

	if (!extensions.empty()) {
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		device_create_info.ppEnabledExtensionNames = extensions.data();
	}

	// Create logical device.
	data_->logical_device = data_->physical_device.createDevice(device_create_info);

	// Create QueueFamily handles.
	for (const QueueFamilyProperties& config : queue_family_configs) {
		data_->queue_families_.emplace_back(handle_manager_->createHandle<QueueFamily>(data_->logical_device, config));
	}

	// Create Allocation and Program manager
	data_->allocation_manager = handle_manager_->createHandle<AllocationManager>(data_->physical_device, data_->logical_device);
	data_->program_manager = handle_manager_->createHandle<ProgramManager>(data_->logical_device);

	// Mark the device as initialized.
	data_->initialized_ = true;
}
#pragma endregion


std::vector<vk::ExtensionProperties> VulkanDevice::supportedExtensions() const {
	return data_->physical_device.enumerateDeviceExtensionProperties();
}

vk::PhysicalDeviceProperties VulkanDevice::properties() const {
	return data_->physical_device.getProperties();
}

vk::PhysicalDeviceFeatures VulkanDevice::features() const {
	return data_->physical_device.getFeatures();
}

vk::PhysicalDeviceMemoryProperties VulkanDevice::memoryProperties() const {
	return data_->physical_device.getMemoryProperties();
}

std::vector<QueueFamilyProperties> VulkanDevice::queueFamilyProperties(const vk::SurfaceKHR& surface) const {
	std::vector<vk::QueueFamilyProperties> vk_properties = data_->physical_device.getQueueFamilyProperties();

	// Create configurable QueueFamilyProperties.
	std::vector<QueueFamilyProperties> properties{};
	properties.reserve(vk_properties.size());

	for (size_t i = 0u; i < vk_properties.size(); ++i) {
		properties.emplace_back(static_cast<uint32_t>(i), vk_properties[i], surface && data_->physical_device.getSurfaceSupportKHR(i, surface));
	}

	return properties;
}

const QueueFamily& VulkanDevice::getQueueFamily(uint32_t index) const {
	if (!data_->initialized_) {
		throw std::runtime_error("Tried to retrieve QueueFamily from an uninitialized VulkanDevice.");
	}

	auto it = std::find_if(data_->queue_families_.begin(), data_->queue_families_.end(), [index](const QueueFamily& family) {
		return family.properties().family_index == index;
	});

	// Check if the queue family was found.
	if (it == data_->queue_families_.end()) {
		throw "Queue family index out of range.";
	}

	return *it;
}


DescriptorPool VulkanDevice::createDescriptorPool(const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags) {
	if (!data_->initialized_) {
		throw std::runtime_error("Tried to create DescriptorPool for an uninitialized VulkanDevice.");
	}

	return handle_manager_->createHandle<DescriptorPool>(data_->logical_device, pool_sizes, flags);
}

const ProgramManager& VulkanDevice::getProgramManager() const {
	return data_->program_manager;
}

const AllocationManager& VulkanDevice::getAllocationManager() const {
	return data_->allocation_manager;
}

void VulkanDevice::executeDescriptorsUpdate(const DescriptorsUpdate& update) const {
	data_->logical_device.updateDescriptorSets(update.getWriteOperations(), update.getCopyOperations());
}

bool VulkanDevice::initialized() const {
	return data_->initialized_;
}

#pragma region DEVICE HANDLE GETTERS
const vk::PhysicalDevice& VulkanDevice::getPhysicalDeviceHandle() const {
	return data_->physical_device;
}

const vk::Device& VulkanDevice::getLogicalDeviceHandle() const {
	return data_->logical_device;
}

void VulkanDevice::free() {
	if (data_->initialized_) {
		handle_manager_->destroyAllHandles();
		data_->logical_device.destroy();
	}

	DependentDestroyableHandle::free();
}
#pragma endregion


VulkanDevice::DeviceData::DeviceData(const vk::PhysicalDevice& physical_device)
	: physical_device(physical_device) {
}

} /// !namespace logi
