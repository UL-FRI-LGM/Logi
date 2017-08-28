#include "vk_queue.h"
#include <vulkan/vulkan.hpp>

namespace vkr {

VulkanQueue::VulkanQueue(VulkanQueueFamily &owner_queue_family, VkQueue queue) : owner_queue_family_(owner_queue_family), queue_(queue) {
}

VulkanQueue::~VulkanQueue() {}

}