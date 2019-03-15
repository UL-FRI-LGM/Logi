#include "logi/base/PhysicalDevice.h"

namespace logi {

PhysicalDevice::PhysicalDevice(const VulkanInstance& instance, const vk::PhysicalDevice& vk_physical_device)
  : OwnedHandle<VulkanInstance>(instance),
    vk_physical_device_(std::make_shared<vk::PhysicalDevice>(vk_physical_device)) {}

vk::PhysicalDeviceProperties PhysicalDevice::properties() const {
  checkForNullHandleInvocation("PhysicalDevice", "properties");
  return vk_physical_device_->getProperties();
}

vk::PhysicalDeviceFeatures PhysicalDevice::features() const {
  checkForNullHandleInvocation("PhysicalDevice", "features");
  return vk_physical_device_->getFeatures();
}

std::vector<vk::ExtensionProperties> PhysicalDevice::supportedExtensions() const {
  checkForNullHandleInvocation("PhysicalDevice", "supportedExtensions");
  return vk_physical_device_->enumerateDeviceExtensionProperties();
}

vk::PhysicalDeviceMemoryProperties PhysicalDevice::memoryProperties() const {
  checkForNullHandleInvocation("PhysicalDevice", "memoryProperties");
  return vk_physical_device_->getMemoryProperties();
}

std::vector<QueueFamilyProperties> PhysicalDevice::queueFamilyProperties() const {
  checkForNullHandleInvocation("PhysicalDevice", "queueFamilyProperties");
  std::vector<vk::QueueFamilyProperties> vk_properties = vk_physical_device_->getQueueFamilyProperties();

  // Create configurable QueueFamilyProperties.
  std::vector<QueueFamilyProperties> properties{};
  properties.reserve(vk_properties.size());

  for (size_t i = 0u; i < vk_properties.size(); ++i) {
    properties.emplace_back(static_cast<uint32_t>(i), vk_properties[i]);
  }

  return properties;
}

bool PhysicalDevice::supportsSurfacePresent(const vk::SurfaceKHR& surface, const uint32_t family_index) const {
  checkForNullHandleInvocation("PhysicalDevice", "supportsSurfacePresent");
  return vk_physical_device_->getSurfaceSupportKHR(family_index, surface);
}

bool PhysicalDevice::supportsSurfacePresent(const vk::SurfaceKHR& surface,
                                            const QueueFamilyProperties& family_properties) const {
  checkForNullHandleInvocation("PhysicalDevice", "supportsSurfacePresent");
  return vk_physical_device_->getSurfaceSupportKHR(family_properties.family_index, surface);
}

const LogicalDevice& PhysicalDevice::createLogicalDevice(const LogicalDeviceConfig& config) const {
  checkForNullHandleInvocation("PhysicalDevice", "createLogicalDevice");
  return HandleGenerator<PhysicalDevice, LogicalDevice>::createHandle(config);
}

PhysicalDevice::operator vk::PhysicalDevice() const {
  checkForNullHandleInvocation("PhysicalDevice", "operator vk::PhysicalDevice()");
  return *vk_physical_device_;
}

void PhysicalDevice::free() {
  HandleGenerator<PhysicalDevice, LogicalDevice>::destroyAllHandles();
}

} // namespace logi
