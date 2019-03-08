#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include "logi/memory/Buffer.h"

namespace logi {

/*
std::vector<unsigned char> Buffer::getData() {
	vk::DeviceMemory memory = vk::DeviceMemory(allocation_->GetMemory());
	void* mapped_memory = device_.mapMemory(memory, 0, configuration_.buffer_size);

	std::vector<unsigned char> data;
	data.resize(configuration_.buffer_size);
	std::memcpy(data.data(), mapped_memory, data.size());

	device_.unmapMemory(memory);

	return data;
}
*/

void Buffer::writeData(void* data, size_t size_bytes) {
	void* mapped_data;
	vmaMapMemory(allocator_, allocation_, &mapped_data);
	memcpy(mapped_data, data, size_bytes);
	vmaUnmapMemory(allocator_, allocation_);
}

BufferConfiguration::BufferConfiguration(vk::DeviceSize size, vk::BufferUsageFlags usage, MemoryUsage memory_usage, std::vector<uint32_t> concurrent_queue_families, vk::BufferCreateFlags flags)
	: flags(flags), size(size), usage(usage), memory_usage(memory_usage), concurrent_queue_families(concurrent_queue_families) {}


Buffer::Buffer() : DependentDestroyableHandle({}, false), allocator_(nullptr), allocation_(nullptr) { }

Buffer::Buffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Buffer& buffer, VmaAllocator allocator, VmaAllocation allocation, const BufferConfiguration& configuration)
	: DependentDestroyableHandle(owner), allocator_(allocator), allocation_(allocation), configuration_(std::make_shared<BufferConfiguration>(configuration)),
	  vk_buffer_(std::make_shared<ManagedVkBuffer>(device, buffer)), buffer_view_hm_(std::make_shared<HandleManager>()) {}

BufferView Buffer::createBufferView(const BufferViewConfiguration& configuration) const {
	if (!alive()) {
		throw std::runtime_error("Called 'createBufferView' on destroyed Image object.");
	}

	// Create and register buffer view handle.
	return buffer_view_hm_->createHandle<BufferView>(vk_buffer_->getOwner(), vk_buffer_->get(), configuration);

}

const vk::Buffer& Buffer::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called 'getVkHandle' on destroyed Buffer object.");
	}

	return vk_buffer_->get();
}

void Buffer::free() {
	if (alive()) {
		// Destroy owned image views, image and free the memory.
		buffer_view_hm_->destroyAllHandles();
		vk_buffer_->destroy();
		vmaFreeMemory(allocator_, allocation_);

		Handle::free();
	}
}

}
