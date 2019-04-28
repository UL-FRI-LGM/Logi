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

#include "logi/instance/vulkan_instance.hpp"
#include "logi/instance/debug_report_callback_impl.hpp"
#include "logi/instance/vulkan_instance_impl.hpp"

namespace logi {

DebugReportCallbackEXT
  VulkanInstance::createDebugReportCallbackEXT(const vk::DebugReportCallbackCreateInfoEXT& create_info,
                                               const std::optional<vk::AllocationCallbacks>& allocator = {}) const {
  return object_->createDebugReportCallbackEXT(create_info, allocator);
}
void VulkanInstance::destroyDebugReportCallbackEXT(const DebugReportCallbackEXT& callback) const {
  object_->destroyDebugReportCallbackEXT(callback.id());
}

std::vector<PhysicalDevice> VulkanInstance::enumeratePhysicalDevices() const {
  return object_->enumeratePhysicalDevices();
}

const vk::DispatchLoaderDynamic& VulkanInstance::getDispatcher() const {
  return object_->getDispatcher();
}

VulkanInstance::operator vk::Instance() const {
  return object_->operator vk::Instance();
}

void VulkanInstance::destroy() {
  object_->destroy();
}

VulkanInstance createInstance(const vk::InstanceCreateInfo& create_info, PFN_vkCreateInstance pfn_create_instance,
                              PFN_vkGetInstanceProcAddr pfn_get_proc_addr,
                              const std::optional<vk::AllocationCallbacks>& allocator) {
  return VulkanInstance(
    std::make_shared<VulkanInstanceImpl>(create_info, pfn_create_instance, pfn_get_proc_addr, allocator));
}

bool checkValidationLayerSupport(const std::vector<const char*>& layers) {
  // Fetch validation layers
  std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();

  // Check if all requested validation layers are present among the available layers.
  for (const char* layer : layers) {
    bool found = false;

    for (const auto& layer_properties : available_layers) {
      if (strcmp(layer, layer_properties.layerName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      return false;
    }
  }

  return true;
}

} // namespace logi
