#include "memory/Buffer.h"

namespace vkr {

BufferConfiguration::BufferConfiguration(vk::DeviceSize buffer_size, vk::BufferUsageFlags usage, MemoryUsage memory_usage, bool concurrent, std::vector<uint32_t> concurrent_queue_families)
	: buffer_size(buffer_size), usage(usage), memory_usage(memory_usage), concurrent(concurrent), concurrent_queue_families(concurrent_queue_families) {
}

Buffer::Buffer(const vk::Buffer& buffer, const VmaAllocation& allocation, const BufferConfiguration& configuration)
	: buffer_(buffer), allocation_(allocation), configuration_(configuration) {
}

const vk::Buffer& Buffer::getVkBuffer() {
	return buffer_;
}

const VmaAllocation& Buffer::getAllocation() {
	return allocation_;
}


}