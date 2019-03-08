#include "logi/memory/BufferView.h"

namespace logi {

BufferViewConfiguration::BufferViewConfiguration(vk::Format format, vk::DeviceSize offset, vk::DeviceSize range)
	: format(format), offset(offset), range(range) { }


BufferView::BufferView(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::Buffer& vk_buffer, const BufferViewConfiguration& configuration)
	: DependentDestroyableHandle(owner), configuration_(std::make_shared<BufferViewConfiguration>(configuration)), vk_buffer_view_(nullptr) {

	const vk::BufferViewCreateInfo bv_create_info(vk::BufferViewCreateFlags(), vk_buffer, configuration_->format, configuration_->offset, configuration_->range);
	vk_buffer_view_ = std::make_shared<ManagedVkBufferView>(vk_device, vk_device.createBufferView(bv_create_info));
}

const BufferViewConfiguration& BufferView::configuration() const {
	return *configuration_;
}

const vk::BufferView& BufferView::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called 'getVkHandle' on destroyed BufferView object.");
	}

	return vk_buffer_view_->get();
}

void BufferView::free() {
	vk_buffer_view_->destroy();
	Handle::free();
}


}
