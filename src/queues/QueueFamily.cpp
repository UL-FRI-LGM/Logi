#include "queues/QueueFamily.h"

namespace vkr {

Queue::Queue(const vk::Queue& queue) : queue_(queue) {
}

void Queue::submit(const std::vector<vk::SubmitInfo>& submit_infos, Fence* fence) {
	if (fence == nullptr) {
		queue_.submit(submit_infos, nullptr);
	}
	else {
		queue_.submit(submit_infos, fence->getVkHandle());
	}
}

void Queue::waitIdle() {
	queue_.waitIdle();
}


QueueFamily::QueueFamily(uint32_t family_index, vk::QueueFlags queue_flags, size_t max_queue_count, uint32_t timestamp_valid_bits, vk::Extent3D min_image_transfer_granularity)
	: family_index_(family_index), queue_flags_(queue_flags), max_queue_count_(max_queue_count), timestamp_valid_bits_(timestamp_valid_bits), min_image_transfer_granularity_(min_image_transfer_granularity), device_(nullptr), initialized_(false), queues_(), command_pool_(nullptr) {
}

void QueueFamily::initialize(const vk::Device& device, uint32_t queue_count) {
	device_ = device;

	// Fetch queues.
	queues_.reserve(queue_count);

	for (size_t i = 0; i < queue_count; i++) {
		queues_.emplace_back(std::make_unique<Queue>(device_.getQueue(family_index_, i)));
	}
}

CommandPool* QueueFamily::createCommandPool(bool transistent, bool resetable_buffers) {
	if (command_pool_.get() != nullptr) {
		throw std::runtime_error("Command pool is already created.");
	}

	command_pool_ = std::make_unique<CommandPool>(device_, family_index_, transistent, resetable_buffers);
}

CommandPool* QueueFamily::getCommandPool() {
	return command_pool_.get();
}

uint32_t QueueFamily::getFamilyIndex() const {
	return family_index_;
}

uint32_t QueueFamily::getTimestampValidBits() const {
	return timestamp_valid_bits_;
}

const vk::Extent3D& QueueFamily::getMinImageTransferGranularity() const {
	return min_image_transfer_granularity_;
}

const uint32_t QueueFamily::getMaxSupportedQueueCount() const {
	return max_queue_count_;
}

size_t QueueFamily::queueCount() {
	return queues_.size();
}

Queue* QueueFamily::getQueue(size_t index) {
	return queues_[index].get();
}

QueueFamily::~QueueFamily() {
	queues_.clear();
	command_pool_.reset();
}

}