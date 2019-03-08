/*
* Vulkan Device class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include <vulkan/vulkan.hpp>
#include "logi/base/LogicalDevice.h"

namespace logi {

#pragma region INITIALIZATION

LogicalDeviceConfig::LogicalDeviceConfig(std::vector<QueueFamilyConfig> queues_config, std::vector<const char*> extensions,
	                                     vk::PhysicalDeviceFeatures enabled_features, const vk::DeviceCreateFlags& flags) 
    : queues_config(std::move(queues_config)), extensions(std::move(extensions)), 
      enabled_features(std::move(enabled_features)), flags(flags) {}

vk::DeviceCreateInfo LogicalDeviceConfig::build() {
  // Generate queue infos.
	vk_queue_infos_.clear();
	vk_queue_infos_.reserve(queues_config.size());

  for (const QueueFamilyConfig& config : queues_config) {
		vk_queue_infos_.emplace_back(config.build());
  }

  // Build device create info.
	vk::DeviceCreateInfo create_info(flags, static_cast<uint32_t>(vk_queue_infos_.size()), vk_queue_infos_.data(), 0, nullptr, static_cast<uint32_t>(extensions.size()),
		                             extensions.data(), &enabled_features);
	create_info.pNext = buildExtensions();

	return create_info;
}


LogicalDevice::LogicalDevice() : DependentDestroyableHandle({}, false), data_(nullptr), handle_manager_(nullptr) {}

LogicalDevice::LogicalDevice(const std::weak_ptr<HandleManager>& owner, vk::PhysicalDevice& device, const LogicalDeviceConfig& configuration)
	: DependentDestroyableHandle(owner), data_(std::make_shared<DeviceData>(device, configuration)), 
      handle_manager_(std::make_shared<HandleManager>()) {

	// Create logical device.
	data_->logical_device = data_->physical_device.createDevice(data_->configuration.build());

	// Create QueueFamily handles.
	for (const QueueFamilyConfig& config : data_->configuration.queues_config) {
		if (config.queue_count > 0) {
			data_->queue_families.emplace_back(handle_manager_->createHandle<QueueFamily>(data_->logical_device, config));
		}
	}

	// Create Allocation and Program manager
	data_->allocation_manager = handle_manager_->createHandle<AllocationManager>(data_->physical_device, data_->logical_device);
	data_->program_manager = handle_manager_->createHandle<ProgramManager>(data_->logical_device);
}

#pragma endregion


const QueueFamily& LogicalDevice::getQueueFamily(uint32_t index) const {
    const auto it = std::find_if(data_->queue_families.begin(), data_->queue_families.end(), [index](const QueueFamily& family) {
		return family.configuration().properties.family_index == index;
	});

	// Check if the queue family was found.
	if (it == data_->queue_families.end()) {
		throw std::runtime_error("Queue family index out of range.");
	}

	return *it;
}

const std::vector<QueueFamily>& LogicalDevice::getQueueFamilies() const {
	return data_->queue_families;
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


LogicalDevice::DeviceData::DeviceData(vk::PhysicalDevice physical_device, LogicalDeviceConfig configuration) 
	: physical_device(std::move(physical_device)), configuration(std::move(configuration)) {}





} /// !namespace logi
