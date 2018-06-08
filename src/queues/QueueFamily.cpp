#include "queues/QueueFamily.h"

namespace logi {


QueueFamilyProperties::QueueFamilyProperties(const uint32_t family_index, const vk::QueueFamilyProperties& properties, const bool supports_present)
	: family_index(family_index), queue_flags(properties.queueFlags), max_queue_count(properties.queueCount), timestamp_valid_bits(properties.timestampValidBits),
	min_image_transfer_granularity(properties.minImageTransferGranularity), supports_present(supports_present) {}

void QueueFamilyProperties::configure(const uint32_t queue_count, const vk::DeviceQueueCreateFlags& create_flags, std::optional<const std::vector<float>> queue_priorities) {
	this->create_flags = create_flags;
	this->queue_count = queue_count;

	// If queue properties are not provided set all to 0.
	if (queue_priorities.has_value()) {
		this->queue_priorities = queue_priorities.value();
	} else {
		this->queue_priorities = std::vector<float>(queue_count, 0.0f);
	}
}


QueueFamily::QueueFamily(const vk::Device device, const QueueFamilyProperties& configuration)
	: data_(std::make_shared<QueueFamilyData>(device, configuration)) {

	// Create queues.
	data_->queues.reserve(configuration.queue_count);
	for (size_t i = 0; i < configuration.queue_count; i++) {
		data_->queues.emplace_back(data_->handle_manager.createHandle<Queue>(data_->vk_device.getQueue(configuration.family_index, i)));
	}
}

CommandPool QueueFamily::createCommandPool(const vk::CommandPoolCreateFlags& flags) const {
	return data_->handle_manager.createHandle<CommandPool>(data_->vk_device, data_->configuration.family_index, flags);
}

const QueueFamilyProperties& QueueFamily::properties() const {
	return data_->configuration;
}

Queue QueueFamily::getQueue(const size_t index) const {
	return data_->queues[index];
}

void QueueFamily::free() {
	data_->handle_manager.destroyAllHandles();
	Handle::free();
}

QueueFamily::QueueFamilyData::QueueFamilyData(const vk::Device& vk_device, const QueueFamilyProperties& configuration)
	: vk_device(vk_device), configuration(configuration) { }

}
