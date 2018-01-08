#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include "memory/MemoryUsage.h"

namespace vkr {

struct BufferConfiguration {
	vk::DeviceSize buffer_size;
	vk::BufferUsageFlags usage;
	MemoryUsage memory_usage;
	bool concurrent;
	std::vector<uint32_t> concurrent_queue_families;

	BufferConfiguration(vk::DeviceSize buffer_size = 0, vk::BufferUsageFlags usage = vk::BufferUsageFlags(), MemoryUsage memory_usage = MemoryUsage::GPU_ONLY,  bool concurrent = false, std::vector<uint32_t> concurrent_queue_families = {});
};

class Buffer {
public:
	Buffer(const vk::Buffer& buffer, const VmaAllocation& allocation, const BufferConfiguration& configuration);

	const vk::Buffer& getVkBuffer();

	const VmaAllocation& getAllocation();

private:
	vk::Buffer buffer_;
	VmaAllocation allocation_;
	BufferConfiguration configuration_;
};

}

#endif // !MEMORY_BUFFER_H
