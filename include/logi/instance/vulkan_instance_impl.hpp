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
  /**
   * @brief Create Vulkan instance.
   *
   * @param	create_info Structure specifying instance parameters.
   */
  explicit VulkanInstanceImpl(const vk::InstanceCreateInfo& create_info,
                              PFN_vkCreateInstance pfn_create_instance = &vkCreateInstance,
                              PFN_vkGetInstanceProcAddr pfn_get_proc_addr = &vkGetInstanceProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

  DebugReportCallbackEXT createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& create_info,
                                                      const std::optional<vk::AllocationCallbacks>& allocator);

  void destroyDebugReportCallbackEXT(size_t id);

  SurfaceKHR registerSurfaceKHR(const vk::SurfaceKHR& vk_surface,
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
  vk::Instance vk_instance_;
  vk::DispatchLoaderDynamic dispatcher_;
  std::vector<vk::DebugReportCallbackEXT> debug_callbacks_;
};

} // namespace logi

#endif // LOGI_BASE_VULKAN_INSTANCE_IMPL_HPP
