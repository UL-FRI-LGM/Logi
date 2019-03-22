/*
 * Vulkan Device class.
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BASE_VULKAN_DEVICE_H_
#define BASE_VULKAN_DEVICE_H_

#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/base/SwapChain.h"
#include "logi/descriptors/DecriptorsUpdate.h"
#include "logi/descriptors/DescriptorPool.h"
#include "logi/memory/AllocationManager.h"
#include "logi/memory/Sampler.h"
#include "logi/program/ProgramManager.h"
#include "logi/queues/QueueFamily.h"

namespace logi {

/**
 * @brief	Structure used to specify logical device configuration.
 */
struct LogicalDeviceConfig : public BuildableExtendable {
  /**
   * @brief	Populates LogicalDeviceConfig with the given values.
   *
   * @param	queues_config       Structures describing the queues that are requested to be created along with the logical
   * device.
   * @param	extensions          Null-terminated UTF-8 strings containing the names of extensions to enable for the
   * created device.
   * @param	enabled_features    Structure that contains boolean indicators of all the features to be enabled.
   * @param	flags               Flags used to specify additional configuration.
   */
  explicit LogicalDeviceConfig(std::vector<QueueFamilyConfig> queues_config = {},
                               std::vector<const char*> extensions = {},
                               vk::PhysicalDeviceFeatures enabled_features = {},
                               const vk::DeviceCreateFlags& flags = {});

  /**
   * @brief	Builds Vulkan DeviceCreateInfo structure.
   *
   * @return	Vulkan DeviceCreateInfo structure.
   */
  vk::DeviceCreateInfo build();

  /**
   * Structures describing the queues that are requested to be created along with the logical device.
   */
  std::vector<QueueFamilyConfig> queues_config;

  /**
   * Null-terminated UTF-8 strings containing the names of extensions to enable for the created device.
   */
  std::vector<const char*> extensions;

  /**
   * Structure that contains boolean indicators of all the features to be enabled.
   */
  vk::PhysicalDeviceFeatures enabled_features;

  /**
   * Flags used to specify additional configuration.
   */
  vk::DeviceCreateFlags flags;

 private:
  std::vector<vk::DeviceQueueCreateInfo> vk_queue_infos_;
};

class PhysicalDevice;

/**
 * @brief	Creates VulkanDevice handle used to manage resources of the given Vulkan physical device.
 */
class LogicalDevice : public DestroyableOwnedHandle<LogicalDevice, PhysicalDevice>,
                      public HandleGenerator<LogicalDevice, QueueFamily>,
                      public HandleGenerator<LogicalDevice, AllocationManager>,
                      public HandleGenerator<LogicalDevice, ProgramManager>,
                      public HandleGenerator<LogicalDevice, DescriptorPool>,
                      public HandleGenerator<LogicalDevice, DescriptorSetUpdate>,
                      public HandleGenerator<LogicalDevice, SwapChain>,
                      public HandleGenerator<LogicalDevice, Semaphore>,
                      public HandleGenerator<LogicalDevice, Fence>,
                      public HandleGenerator<LogicalDevice, Sampler> {
 public:
  LogicalDevice() = default;

  /**
   * @brief	Creates VulkanDevice handle used to manage resources of the given Vulkan physical device.
   *
   * @param	physical_device	Physical device handle.
   * @param configuration   Logical device configuration.
   */
  LogicalDevice(const PhysicalDevice& physical_device, const LogicalDeviceConfig& configuration);

  /**
   * @brief   Get queue family with the given index.
   *
   * @param   index Index of the queue family.
   * @return  QueueFamily handle.
   */
  const QueueFamily& getQueueFamily(uint32_t index) const;

  /**
   * @brief   Retrieve queue families of this device
   *
   * @return  QueueFamily handles.
   */
  const std::vector<QueueFamily>& getQueueFamilies() const;

  /**
   * @brief Creates descriptor pool. Allocates enough memory to support creation of as many descriptors of a given type
   * as specified in pool_sizes.
   *
   * @param pool_sizes Contains number of sets and descriptors of different types.
   * @param releasable_sets If true the allocated descriptor sets can be released.
   */
  DescriptorPool createDescriptorPool(const DescriptorsCount& pool_sizes,
                                      const vk::DescriptorPoolCreateFlags& flags = {}) const;

  DescriptorSetUpdate createDescriptorUpdate() const;

  const ProgramManager& getProgramManager() const;

  const AllocationManager& getAllocationManager() const;

  /**
   * @brief Retrieve physical device handle.
   *
   * @return Physical device handle.
   */
  vk::PhysicalDevice getPhysicalDeviceHandle() const;

  /**
   * @brief Retrieve logical device handle.
   *
   * @return Logical device handle.
   */
  vk::Device getLogicalDeviceHandle() const;

  SwapChain createSwapchain(const vk::SurfaceKHR& surface, uint32_t present_family,
                            const std::vector<uint32_t>& concurrent_families = {}) const;

  Semaphore createSemaphore(const vk::SemaphoreCreateFlags& flags = {}) const;

  Fence createFence(const vk::FenceCreateFlags& flags = {}) const;

  void waitIdle() const;

  vk::Result waitForFences(const std::vector<Fence>& fences, bool wait_all, uint64_t timeout) const;

  Sampler createSampler(const SamplerConfiguration& config) const;

  operator vk::Device() const;

 protected:
  /**
   * @brief Frees resources.
   */
  void free() override;

 private:
  struct DeviceData {
    DeviceData(LogicalDeviceConfig configuration);

    /**
     * Logical device handle.
     */
    vk::Device logical_device{};

    /**
     * Logical device configuration.
     */
    LogicalDeviceConfig configuration;

    /**
     * Program manager.
     */
    ProgramManager program_manager{};

    /**
     * Allocation manager.
     */
    AllocationManager allocation_manager{};

    /**
     * Queue families.
     */
    std::vector<QueueFamily> queue_families{};
  };

  std::shared_ptr<DeviceData> data_;
};

} // namespace logi

#endif // !VULKAN_DEVICE_H_
