#include "memory/Buffer.h"

namespace vkr {

BufferConfiguration::BufferConfiguration(vk::DeviceSize buffer_size, vk::BufferUsageFlags usage, MemoryUsage memory_usage, bool concurrent, std::vector<uint32_t> concurrent_queue_families)
	: buffer_size(buffer_size), usage(usage), memory_usage(memory_usage), concurrent(concurrent), concurrent_queue_families(concurrent_queue_families) {
}

BufferView::BufferView(const vk::Device& device, const vk::Buffer& buffer, vk::Format format, vk::DeviceSize offset, vk::DeviceSize range) : device_(device), format_(format), offset_(offset), range_(range) {
	vk::BufferViewCreateInfo buff_view_ci{};
	buff_view_ci.buffer = buffer;
	buff_view_ci.format = format_;
	buff_view_ci.offset = offset_;
	buff_view_ci.range = range_;

	buffer_view_ = device_.createBufferView(buff_view_ci);
}

const vk::BufferView& BufferView::getVkBufferView() {
	return buffer_view_;
}

BufferView::~BufferView() {
	device_.destroyBufferView(buffer_view_);
}



Buffer::Buffer(const vk::Device& device, const vk::Buffer& buffer, const BufferConfiguration& configuration)
	: device_(device), buffer_(buffer), configuration_(configuration) {
}

BufferView* Buffer::createBufferView(vk::Format format, vk::DeviceSize offset, vk::DeviceSize range) {
	if (!(configuration_.usage & vk::BufferUsageFlagBits::eStorageTexelBuffer) && !(configuration_.usage & vk::BufferUsageFlagBits::eUniformTexelBuffer)) {
		throw std::runtime_error("Tried to create buffer view on non texel based buffer!");
	}

	buffer_views_.emplace_back(std::make_unique<BufferView>(device_, buffer_, format, offset, range));
	return buffer_views_.back().get();
}

void Buffer::destroyBufferView(BufferView* buffer_view) {
	auto it = std::find_if(buffer_views_.begin(), buffer_views_.end(), [&buffer_view](const std::unique_ptr<BufferView>& entry) {
		return entry.get() == buffer_view;
	});

	// Destroy buffer view.
	if (it != buffer_views_.end()) {
		buffer_views_.erase(it);
	}
}


const vk::Buffer& Buffer::getVkBuffer() {
	return buffer_;
}


Buffer::~Buffer() {
	buffer_views_.clear();
	device_.destroyBuffer(buffer_);
}

}