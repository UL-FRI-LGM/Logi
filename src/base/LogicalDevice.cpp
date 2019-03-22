/*
 * Vulkan Device class.
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "logi/base/LogicalDevice.h"
#include <vulkan/vulkan.hpp>
#include "logi/base/PhysicalDevice.h"

namespace logi {

#pragma region INITIALIZATION

LogicalDeviceConfig::LogicalDeviceConfig(std::vector<QueueFamilyConfig> queues_config,
                                         std::vector<const char*> extensions,
                                         vk::PhysicalDeviceFeatures enabled_features,
                                         const vk::DeviceCreateFlags& flags)
  : queues_config(std::move(queues_config)), extensions(std::move(extensions)), enabled_features(enabled_features),
    flags(flags) {}

vk::DeviceCreateInfo LogicalDeviceConfig::build() {
  // Generate queue infos.
  vk_queue_infos_.clear();
  vk_queue_infos_.reserve(queues_config.size());

  for (const QueueFamilyConfig& config : queues_config) {
    vk_queue_infos_.emplace_back(config.build());
  }

  // Build device create info.
  vk::DeviceCreateInfo create_info(flags, static_cast<uint32_t>(vk_queue_infos_.size()), vk_queue_infos_.data(), 0,
                                   nullptr, static_cast<uint32_t>(extensions.size()), extensions.data(),
                                   &enabled_features);
  create_info.pNext = buildExtensions();

  return create_info;
}

LogicalDevice::LogicalDevice(const PhysicalDevice& physical_device, const LogicalDeviceConfig& configuration)
  : DestroyableOwnedHandle<LogicalDevice, PhysicalDevice>(physical_device, true),
    data_(std::make_shared<DeviceData>(configuration)) {
  // Create logical device.
  data_->logical_device = static_cast<vk::PhysicalDevice>(physical_device).createDevice(data_->configuration.build());

  // Create QueueFamily handles.
  for (const QueueFamilyConfig& config : data_->configuration.queues_config) {
    if (config.queue_count > 0) {
      data_->queue_families.emplace_back(HandleGenerator<LogicalDevice, QueueFamily>::createHandle(config));
    }
  }

  // Create Allocation and Program manager
  data_->allocation_manager = HandleGenerator<LogicalDevice, AllocationManager>::createHandle();
  data_->program_manager = HandleGenerator<LogicalDevice, ProgramManager>::createHandle();
}

#pragma endregion

const QueueFamily& LogicalDevice::getQueueFamily(uint32_t index) const {
  const auto it =
    std::find_if(data_->queue_families.begin(), data_->queue_families.end(), [index](const QueueFamily& family) {
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

DescriptorPool LogicalDevice::createDescriptorPool(const DescriptorsCount& pool_sizes,
                                                   const vk::DescriptorPoolCreateFlags& flags) const {
  return HandleGenerator<LogicalDevice, DescriptorPool>::createHandle(pool_sizes, flags);
}

DescriptorSetUpdate LogicalDevice::createDescriptorUpdate() const {
  return HandleGenerator<LogicalDevice, DescriptorSetUpdate>::createHandle();
}

const ProgramManager& LogicalDevice::getProgramManager() const {
  return data_->program_manager;
}

const AllocationManager& LogicalDevice::getAllocationManager() const {
  return data_->allocation_manager;
}

#pragma region DEVICE HANDLE GETTERS
vk::PhysicalDevice LogicalDevice::getPhysicalDeviceHandle() const {
  return getOwner<PhysicalDevice>();
}

vk::Device LogicalDevice::getLogicalDeviceHandle() const {
  return data_->logical_device;
}

SwapChain LogicalDevice::createSwapchain(const vk::SurfaceKHR& surface, uint32_t present_family,
                                         const std::vector<uint32_t>& concurrent_families) const {
  return HandleGenerator<LogicalDevice, SwapChain>::createHandle(surface, present_family, concurrent_families);
}

Semaphore LogicalDevice::createSemaphore(const vk::SemaphoreCreateFlags& flags) const {
  return HandleGenerator<LogicalDevice, Semaphore>::createHandle(flags);
}

Fence LogicalDevice::createFence(const vk::FenceCreateFlags& flags) const {
  return HandleGenerator<LogicalDevice, Fence>::createHandle(flags);
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
  return HandleGenerator<LogicalDevice, Sampler>::createHandle(config);
}

LogicalDevice::operator vk::Device() const {
  return data_->logical_device;
}

void LogicalDevice::free() {
  HandleGenerator<LogicalDevice, Semaphore>::destroyAllHandles();
  HandleGenerator<LogicalDevice, Fence>::destroyAllHandles();
  HandleGenerator<LogicalDevice, Sampler>::destroyAllHandles();
  HandleGenerator<LogicalDevice, SwapChain>::destroyAllHandles();
  HandleGenerator<LogicalDevice, DescriptorSetUpdate>::destroyAllHandles();
  HandleGenerator<LogicalDevice, DescriptorPool>::destroyAllHandles();
  HandleGenerator<LogicalDevice, AllocationManager>::destroyAllHandles();
  HandleGenerator<LogicalDevice, ProgramManager>::destroyAllHandles();
  HandleGenerator<LogicalDevice, QueueFamily>::destroyAllHandles();

  data_->logical_device.destroy();

  DestroyableOwnedHandle<LogicalDevice, PhysicalDevice>::free();
}

#pragma endregion

LogicalDevice::DeviceData::DeviceData(LogicalDeviceConfig configuration) : configuration(std::move(configuration)) {}

} // namespace logi
