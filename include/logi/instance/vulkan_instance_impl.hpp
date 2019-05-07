/**
 * Project Logi source code
 * Copyright (C) 2019 Primoz Lavric
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP
#define LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP

#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/base/vulkan_object.hpp"

namespace logi {

class DebugReportCallbackEXT;
class DebugReportCallbackEXTImpl;
class PhysicalDevice;
class PhysicalDeviceImpl;
class SurfaceKHR;
class SurfaceKHRImpl;

/**
 * @brief	Vulkan instance handle.
 */
class VulkanInstanceImpl : public VulkanObject<VulkanInstanceImpl>,
                           public VulkanObjectComposite<DebugReportCallbackEXTImpl>,
                           public VulkanObjectComposite<PhysicalDeviceImpl>,
                           public VulkanObjectComposite<SurfaceKHRImpl> {
 public:
  explicit VulkanInstanceImpl(const vk::InstanceCreateInfo& createInfo,
                              PFN_vkCreateInstance pfnCreateInstance = &vkCreateInstance,
                              PFN_vkGetInstanceProcAddr pfnGetProcAddr = &vkGetInstanceProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  DebugReportCallbackEXT createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& createInfo,
                                                      const std::optional<vk::AllocationCallbacks>& allocator);

  void debugReportMessageEXT(const vk::DebugReportFlagsEXT& flags, vk::DebugReportObjectTypeEXT objectType,
                             uint64_t object, size_t location, int32_t messageCode, const char* layerPrefix,
                             const char* message) const;

  void destroyDebugReportCallbackEXT(size_t id);

  SurfaceKHR registerSurfaceKHR(const vk::SurfaceKHR& vkSurface,
                                const std::optional<vk::AllocationCallbacks>& allocator);

  void destroySurfaceKHR(size_t id);

  std::vector<PhysicalDevice> enumeratePhysicalDevices() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator vk::Instance() const;

  void destroy();

 protected:
  void free() override;

 private:
  std::optional<vk::AllocationCallbacks> allocator_;
  vk::Instance vkInstance_;
  vk::DispatchLoaderDynamic dispatcher_;
  std::vector<vk::DebugReportCallbackEXT> debugCallbacks_;
};

} // namespace logi

#endif // LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP
