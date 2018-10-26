#include "base/PhysicalDevice.h"

namespace logi {

PhysicalDevice::PhysicalDevice() : Handle(false), vk_physical_device_(nullptr), handle_manager_(nullptr) {}

PhysicalDevice::PhysicalDevice(const vk::PhysicalDevice& vk_physical_device)
	: Handle(true), vk_physical_device_(std::make_shared<vk::PhysicalDevice>(vk_physical_device)), handle_manager_(std::make_shared<HandleManager>()) {}


vk::PhysicalDeviceProperties PhysicalDevice::properties() const {
	return vk_physical_device_->getProperties();
}

vk::PhysicalDeviceFeatures PhysicalDevice::features() const {
	return vk_physical_device_->getFeatures();
}

std::vector<vk::ExtensionProperties> PhysicalDevice::supportedExtensions() const {
	return vk_physical_device_->enumerateDeviceExtensionProperties();
}

vk::PhysicalDeviceMemoryProperties PhysicalDevice::memoryProperties() const {
	return vk_physical_device_->getMemoryProperties();
}

std::vector<QueueFamilyProperties> PhysicalDevice::queueFamilyProperties() const {
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
	return vk_physical_device_->getSurfaceSupportKHR(family_index, surface);
}

bool PhysicalDevice::supportsSurfacePresent(const vk::SurfaceKHR& surface, const QueueFamilyProperties& family_properties) const {
	return vk_physical_device_->getSurfaceSupportKHR(family_properties.family_index, surface);
}

LogicalDevice PhysicalDevice::createLogicalDevice(const LogicalDeviceConfig& config) const {
	return handle_manager_->createHandle<LogicalDevice>(*vk_physical_device_, config);
}

void PhysicalDevice::free() {
	handle_manager_->destroyAllHandles();
}


}
