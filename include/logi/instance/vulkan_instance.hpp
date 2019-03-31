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

#ifndef LOGI_BASE_VULKAN_INSTANCE_HPP
#define LOGI_BASE_VULKAN_INSTANCE_HPP

#include <optional>
#include <vulkan/vulkan.hpp>
#include "logi/base/handle.hpp"
#include "logi/device/physical_device.hpp"
#include "logi/instance/debug_report_callback.hpp"

namespace logi {

class VulkanInstanceImpl;

class VulkanInstance : public Handle<VulkanInstanceImpl> {
 public:
  using Handle::Handle;

  DebugReportCallbackEXT createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& create_info,
                                                      const std::optional<vk::AllocationCallbacks>& allocator) const;

  void destroyDebugReportCallbackEXT(const DebugReportCallbackEXT& callback) const;

  std::vector<PhysicalDevice> enumeratePhysicalDevices() const;

  const vk::DispatchLoaderDynamic& getDispatcher() const;

  operator const vk::Instance&() const;

  void destroy();
};

VulkanInstance createInstance(const vk::InstanceCreateInfo& create_info,
                              PFN_vkCreateInstance pfn_create_instance = &vkCreateInstance,
                              PFN_vkGetInstanceProcAddr pfn_get_proc_addr = &vkGetInstanceProcAddr,
                              const std::optional<vk::AllocationCallbacks>& allocator = {});

bool checkValidationLayerSupport(const std::vector<const char*>& layers);

} // namespace logi

#endif // LOGI_BASE_VULKAN_INSTANCE_HPP