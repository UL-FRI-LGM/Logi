#include "queues/QueueFamily.h"

namespace logi {


QueueFamilyProperties::QueueFamilyProperties(const uint32_t family_index, const vk::QueueFamilyProperties& properties)
	: family_index(family_index), queue_flags(properties.queueFlags), max_queue_count(properties.queueCount), timestamp_valid_bits(properties.timestampValidBits),
	min_image_transfer_granularity(properties.minImageTransferGranularity) {}


QueueFamilyConfig::QueueFamilyConfig(const QueueFamilyProperties& properties, uint32_t queue_count, const std::vector<float>& priorities, const vk::DeviceQueueCreateFlags& flags)
	: properties(properties), flags(flags), queue_count(queue_count), priorities(priorities) {}



QueueFamily::QueueFamily(const vk::Device device, const QueueFamilyConfig& configuration)
	: data_(std::make_shared<QueueFamilyData>(device, configuration)), handle_manager_(std::make_shared<HandleManager>()) {

	// Create queues.
	data_->queues.reserve(configuration.queue_count);
	for (size_t i = 0; i < configuration.queue_count; i++) {
		data_->queues.emplace_back(handle_manager_->createHandle<Queue>(data_->vk_device.getQueue(data_->configuration.properties.family_index, i)));
	}
}

CommandPool QueueFamily::createCommandPool(const vk::CommandPoolCreateFlags& flags) const {
	return handle_manager_->createHandle<CommandPool>(data_->vk_device, data_->configuration.properties.family_index, flags);
}

const QueueFamilyConfig& QueueFamily::configuration() const {
	return data_->configuration;
}

Queue QueueFamily::getQueue(const size_t index) const {
	return data_->queues[index];
}

void QueueFamily::free() {
	handle_manager_->destroyAllHandles();
	Handle::free();
}

QueueFamily::QueueFamilyData::QueueFamilyData(const vk::Device& vk_device, const QueueFamilyConfig& config)
	: vk_device(vk_device), configuration(config) { }

}
