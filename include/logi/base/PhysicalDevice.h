/*
 * Vulkan Physical Device class.
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BASE_PHYSICAL_DEVICE_H
#define BASE_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/LogicalDevice.h"
#include "logi/queues/QueueFamily.h"

namespace logi {

class VulkanInstance;

class PhysicalDevice : public OwnedHandle<VulkanInstance>, public HandleGenerator<PhysicalDevice, LogicalDevice> {
 public:
  /**
   * @brief	Default constructor for placeholder object.
   */
  PhysicalDevice() = default;

  /**
   * @brief	Constructs PhysicalDevice handle that wraps
   *
   * @param instance            Instance handle.
   * @param	vk_physical_device  Vulkan physical device handle.
   */
  explicit PhysicalDevice(const VulkanInstance& instance, const vk::PhysicalDevice& vk_physical_device);

  /**
   * @brief	Retrieve device properties.
   *
   * @return	Device properties.
   */
  vk::PhysicalDeviceProperties properties() const;

  /**
   * @brief	Retrieve device features.
   *
   * @return	Device features.
   */
  vk::PhysicalDeviceFeatures features() const;

  /**
   * @brief	Retrieve extensions supported by this device.
   *
   * @return	Vector of supported extensions.
   */
  std::vector<vk::ExtensionProperties> supportedExtensions() const;

  /**
   * @brief	Retrieve device memory properties.
   *
   * @return	Device memory properties.
   */
  vk::PhysicalDeviceMemoryProperties memoryProperties() const;

  /**
   * @brief	Retrieve configurable queue family properties.
   *
   * @return	Vector containing configurable queue family properties for all queue families.
   */
  std::vector<QueueFamilyProperties> queueFamilyProperties() const;

  /**
   * @brief	Check if queue family with the given index supports presentation on the given surface.
   *
   * @return	True if presentation is supported.
   */
  bool supportsSurfacePresent(const vk::SurfaceKHR& surface, uint32_t family_index) const;

  /**
   * @brief	Check if queue family with the given properties supports presentation on the given surface.
   *
   * @return	True if presentation is supported.
   */
  bool supportsSurfacePresent(const vk::SurfaceKHR& surface, const QueueFamilyProperties& family_properties) const;

  /**
   * @brief   Create logical device.
   *
   * @param   config  Logical device configuration.
   * @return  Logical device handle.
   */
  const LogicalDevice& createLogicalDevice(const LogicalDeviceConfig& config) const;

  /**
   * @brief   Implicit conversion to vk::PhysicalDevice.
   *
   * @return  vk::PhysicalDevice handle.
   */
  operator vk::PhysicalDevice() const;

 protected:
  /**
   * @brief Destroys all LogicalDevice handles.
   */
  void free() override;

 private:
  /**
   * Vulkan physical device handle.
   */
  std::shared_ptr<vk::PhysicalDevice> vk_physical_device_;
};

} // namespace logi

#endif
