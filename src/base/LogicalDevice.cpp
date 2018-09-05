/*
* Vulkan Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include <vulkan/vulkan.hpp>
#include "base/LogicalDevice.h"

namespace logi {

#pragma region INITIALIZATION
LogicalDevice::LogicalDevice() : DependentDestroyableHandle({}, false), data_(nullptr), handle_manager_(nullptr) {}

LogicalDevice::LogicalDevice(const std::weak_ptr<HandleManager>& owner, vk::PhysicalDevice& device, std::vector<QueueFamilyConfig>& qfamily_configs, const std::vector<const char*>& extensions = {}, const vk::PhysicalDeviceFeatures& features = {})
	: DependentDestroyableHandle(owner), data_(std::make_shared<DeviceData>(device, extensions, features)), handle_manager_(std::make_shared<HandleManager>()) {
	
	// Generate queue create infos.
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
	queue_create_infos.reserve(qfamily_configs.size());

	for (QueueFamilyConfig& config : qfamily_configs) {
		// Ignore configurations with 0 queue count.
		if (config.queue_count > 0) {
			vk::DeviceQueueCreateInfo queue_ci{};
			queue_ci.flags = config.flags;
			queue_ci.queueCount = config.queue_count;
			config.priorities.resize(config.queue_count, 0.0f);
			queue_ci.pQueuePriorities = config.priorities.data();

			queue_create_infos.emplace_back(queue_ci);
		}
	}

	// Construct device create info.
	vk::DeviceCreateInfo device_create_info{};
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());;
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.pEnabledFeatures = &data_->enabled_features;


	if (!extensions.empty()) {
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(data_->enabled_extensions.size());
		device_create_info.ppEnabledExtensionNames = data_->enabled_extensions.data();
	}

	// Create logical device.
	data_->logical_device = data_->physical_device.createDevice(device_create_info);

	// Create QueueFamily handles.
	for (const QueueFamilyConfig& config : qfamily_configs) {
		data_->queue_families_.emplace_back(handle_manager_->createHandle<QueueFamily>(data_->logical_device, config));
	}

	// Create Allocation and Program manager
	data_->allocation_manager = handle_manager_->createHandle<AllocationManager>(data_->physical_device, data_->logical_device);
	data_->program_manager = handle_manager_->createHandle<ProgramManager>(data_->logical_device);
}

#pragma endregion


const QueueFamily& LogicalDevice::getQueueFamily(uint32_t index) const {

	auto it = std::find_if(data_->queue_families_.begin(), data_->queue_families_.end(), [index](const QueueFamily& family) {
		return family.configuration().properties.family_index == index;
	});

	// Check if the queue family was found.
	if (it == data_->queue_families_.end()) {
		throw std::runtime_error("Queue family index out of range.");
	}

	return *it;
}


DescriptorPool LogicalDevice::createDescriptorPool(const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags) const {
	return handle_manager_->createHandle<DescriptorPool>(data_->logical_device, pool_sizes, flags);
}

DescriptorSetUpdate LogicalDevice::createDescriptorUpdate() const {
	return handle_manager_->createHandle<DescriptorSetUpdate>(data_->logical_device);
}

const ProgramManager& LogicalDevice::getProgramManager() const {
	return data_->program_manager;
}

const AllocationManager& LogicalDevice::getAllocationManager() const {
	return data_->allocation_manager;
}

#pragma region DEVICE HANDLE GETTERS
const vk::PhysicalDevice& LogicalDevice::getPhysicalDeviceHandle() const {
	return data_->physical_device;
}

const vk::Device& LogicalDevice::getLogicalDeviceHandle() const {
	return data_->logical_device;
}

SwapChain LogicalDevice::createSwapchain(const vk::SurfaceKHR& surface, uint32_t present_family,
	const std::vector<uint32_t>& concurrent_families) const {
	return handle_manager_->createHandle<SwapChain>(data_->physical_device, data_->logical_device, surface, present_family, concurrent_families);
}

Semaphore LogicalDevice::createSemaphore(const vk::SemaphoreCreateFlags& flags) const {
	return handle_manager_->createHandle<Semaphore>(data_->logical_device, flags);
}

Fence LogicalDevice::createFence(const vk::FenceCreateFlags& flags) const {
	return handle_manager_->createHandle<Fence>(data_->logical_device, flags);
}

void LogicalDevice::waitIdle() const {
	data_->logical_device.waitIdle();
}

vk::Result LogicalDevice::waitForFences(const std::vector<Fence>& fences, bool wait_all, uint64_t timeout) const {
	std::vector<vk::Fence> vk_fences;

	for (const Fence& fence : fences) {
		vk_fences.emplace_back(fence.getVkHandle());
	}

	return data_->logical_device.waitForFences(vk_fences, wait_all, timeout);
}

Sampler LogicalDevice::createSampler(const SamplerConfiguration& config) const {
	return handle_manager_->createHandle<Sampler>(data_->logical_device, config);
}

void LogicalDevice::free() {
	handle_manager_->destroyAllHandles();
	data_->logical_device.destroy();

	DependentDestroyableHandle::free();
}

#pragma endregion


LogicalDevice::DeviceData::DeviceData(const vk::PhysicalDevice& physical_device,
	const std::vector<const char*>& enabled_extensions, const vk::PhysicalDeviceFeatures& enabled_features)
	: physical_device(physical_device), enabled_extensions(enabled_extensions), enabled_features(enabled_features) {}


} /// !namespace logi
