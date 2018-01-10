
#include <queues\QueueManager.h>


vkr::Queue::Queue(const vk::Queue& queue) : queue_(queue) {
}



vkr::QueueManager::QueueManager(const vk::PhysicalDevice& physical_device) : physical_device_(physical_device), logical_device_(nullptr), queue_family_properties_() {
	queue_family_properties_ = physical_device_.getQueueFamilyProperties();

	/*
	for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties_.size()); i++) {
		if (queue_families_[static_cast<size_t>(QueueFamilyType::GRAPHIC)].family_index == UINT_MAX && queue_family_properties_[i].queueFlags & vk::QueueFlagBits::eGraphics) {
			queue_families_[static_cast<size_t>(QueueFamilyType::GRAPHIC)].family_index = i;
		}
		else if (queue_families_[static_cast<size_t>(QueueFamilyType::COMPUTE)].family_index == UINT_MAX && queue_family_properties_[i].queueFlags & vk::QueueFlagBits::eCompute) {
			queue_families_[static_cast<size_t>(QueueFamilyType::COMPUTE)].family_index = i;
		}
		else if (queue_families_[static_cast<size_t>(QueueFamilyType::TRANSFER)].family_index == UINT_MAX && queue_family_properties_[i].queueFlags & vk::QueueFlagBits::eTransfer) {
			queue_families_[static_cast<size_t>(QueueFamilyType::TRANSFER)].family_index = i;
		}
	}*/
}

