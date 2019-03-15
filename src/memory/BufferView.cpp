#include "logi/memory/BufferView.h"
#include "logi/base/LogicalDevice.h"
#include "logi/memory/Buffer.h"

namespace logi {

BufferViewConfiguration::BufferViewConfiguration(vk::Format format, vk::DeviceSize offset, vk::DeviceSize range)
  : format(format), offset(offset), range(range) {}

BufferView::BufferView(const Buffer& buffer, const BufferViewConfiguration& config)
  : DestroyableOwnedHandle<Buffer>(buffer, true) {
  const vk::BufferViewCreateInfo bv_create_info(vk::BufferViewCreateFlags(), buffer, config.format, config.offset,
                                                config.range);

  vk::Device vk_device = getOwner<LogicalDevice>();
  data_ = std::make_shared<Data>(config, ManagedVkBufferView(vk_device, vk_device.createBufferView(bv_create_info)));
}

const BufferViewConfiguration& BufferView::configuration() const {
  checkForNullHandleInvocation("BufferView", "configuration");
  return data_->config;
}

const vk::BufferView& BufferView::getVkHandle() const {
  checkForNullHandleInvocation("BufferView", "getVkHandle");
  return data_->vk_buffer_view.get();
}

BufferView::operator vk::BufferView() const {
  checkForNullHandleInvocation("BufferView", "operator vk::BufferView()");
  return data_->vk_buffer_view.get();
}

void BufferView::free() {
  if (valid()) {
    data_->vk_buffer_view.destroy();
    DestroyableOwnedHandle<Buffer>::free();
  }
}

} // namespace logi
