
#include "memory/AllocationManager.h"

namespace vkr {

VmaMemoryUsage memoryUsageToVma(const MemoryUsage& usage) {
	switch (usage) {
	case MemoryUsage::GPU_ONLY:
		return VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
		break;
	case MemoryUsage::CPU_ONLY:
		return VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_ONLY;
		break;
	case MemoryUsage::CPU_TO_GPU:
		return VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
		break;
	case MemoryUsage::GPU_TO_CPU:
		return VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
		break;
	}
}

AllocationManager::AllocationManager(const vk::PhysicalDevice& physical_device, const vk::Device& logical_device) : allocated_buffers_(), allocated_images_() {
	// Create VMA for the given device.
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = (VkPhysicalDevice) physical_device;
	allocatorInfo.device = (VkDevice) logical_device;

	vmaCreateAllocator(&allocatorInfo, &allocator_);
}

std::shared_ptr<Buffer> AllocationManager::allocateBuffer(const BufferConfiguration& configuration) {
	vk::BufferCreateInfo buffer_info{};
	buffer_info.size = configuration.buffer_size;
	buffer_info.usage = configuration.usage;

	// If the buffer is required to be concurrent add concurrent queue families.
	if (configuration.concurrent) {
		buffer_info.sharingMode = vk::SharingMode::eConcurrent;
		buffer_info.pQueueFamilyIndices = configuration.concurrent_queue_families.data();
		buffer_info.queueFamilyIndexCount = configuration.concurrent_queue_families.size();
	}
	else {
		buffer_info.sharingMode = vk::SharingMode::eExclusive;
	}

	// High level usage type.
	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = memoryUsageToVma(configuration.memory_usage);
	
	// Allocate buffer.
	VkBuffer buffer;
	VmaAllocation allocation;
	if (vmaCreateBuffer(allocator_, &((VkBufferCreateInfo)buffer_info), &allocInfo, &buffer, &allocation, nullptr) != VK_SUCCESS) {
		return nullptr;
	}

	std::shared_ptr<Buffer> p_buffer = std::make_shared<Buffer>(vk::Buffer(buffer), allocation, configuration);
	allocated_buffers_.emplace_back(p_buffer); 

	return p_buffer;
}

void AllocationManager::freeBuffer(const std::shared_ptr<Buffer>& buffer) {
	vmaDestroyBuffer(allocator_, (VkBuffer)buffer->getVkBuffer(), buffer->getAllocation());
	allocated_buffers_.remove(buffer);
}

std::shared_ptr<Image> AllocationManager::allocateImage(const ImageConfiguration& configuration) {
	// Build image create info.
	vk::ImageCreateInfo image_info{};
	image_info.flags = configuration.create_flags;
	image_info.imageType = configuration.type;
	image_info.format = configuration.format;
	image_info.extent = configuration.extent;
	image_info.mipLevels = configuration.mip_levels;
	image_info.arrayLayers = configuration.array_layers;
	image_info.samples = configuration.samples;
	image_info.tiling = configuration.tiling;
	image_info.initialLayout = configuration.initial_layout;

	// If the image is required to be concurrent add concurrent queue families.
	if (configuration.concurrent) {
		image_info.sharingMode = vk::SharingMode::eConcurrent;
		image_info.pQueueFamilyIndices = configuration.concurrent_queue_families.data();
		image_info.queueFamilyIndexCount = configuration.concurrent_queue_families.size();
	}
	else {
		image_info.sharingMode = vk::SharingMode::eExclusive;
	}

	// High level usage type.
	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = memoryUsageToVma(configuration.memory_usage);

	// Allocate Image.
	VkImage image;
	VmaAllocation allocation;
	if (vmaCreateImage(allocator_, &((VkImageCreateInfo) image_info), &allocInfo, &image, &allocation, nullptr) != VK_SUCCESS) {
		return nullptr;
	}

	std::shared_ptr<Image> p_image = std::make_shared<Image>(vk::Image(image), allocation, configuration);
	allocated_images_.emplace_back(p_image);

	return p_image;
}

void AllocationManager::freeImage(const std::shared_ptr<Image>& image) {
	vmaDestroyImage(allocator_, (VkBuffer)image->getVkImage(), image->getAllocation());
	allocated_images_.remove(image);
}

AllocationManager::~AllocationManager() {
	// Destroy buffers.
	for (auto it = allocated_buffers_.begin(); it != allocated_buffers_.end(); it++) {
		vmaDestroyBuffer(allocator_, (VkBuffer) (*it)->getVkBuffer(), (*it)->getAllocation());
	}

	// Destroy images.
	for (auto it = allocated_images_.begin(); it != allocated_images_.end(); it++) {

	}
}

}