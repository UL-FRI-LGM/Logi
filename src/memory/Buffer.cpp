#include <utility>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include "logi/base/LogicalDevice.h"
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
  vmaMapMemory(data_->allocator, data_->allocation, &mapped_data);
  memcpy(mapped_data, data, size_bytes);
  vmaUnmapMemory(data_->allocator, data_->allocation);
}

BufferConfiguration::BufferConfiguration(vk::DeviceSize size, const vk::BufferUsageFlags& usage,
                                         MemoryUsage memory_usage, std::vector<uint32_t> concurrent_queue_families,
                                         const vk::BufferCreateFlags& flags)
  : flags(flags), size(size), usage(usage), memory_usage(memory_usage),
    concurrent_queue_families(std::move(concurrent_queue_families)) {}

Buffer::Buffer(const AllocationManager& alloc_manager, const vk::Buffer& buffer, VmaAllocator allocator,
               VmaAllocation allocation, const BufferConfiguration& configuration)
  : DestroyableOwnedHandle<AllocationManager>(alloc_manager, true),
    data_(std::make_shared<Data>(allocator, allocation, configuration,
                                 ManagedVkBuffer(getOwner<LogicalDevice>().getLogicalDeviceHandle(), buffer))) {}

BufferView Buffer::createBufferView(const BufferViewConfiguration& configuration) const {
  checkForNullHandleInvocation("Buffer", "createBufferView");
  return HandleGenerator<Buffer, BufferView>::createHandle(configuration);
}

const vk::Buffer& Buffer::getVkHandle() const {
  checkForNullHandleInvocation("Buffer", "getVkHandle");
  return data_->vk_buffer.get();
}

Buffer::operator vk::Buffer() const {
  checkForNullHandleInvocation("Buffer", "operator vk::Buffer()");
  return data_->vk_buffer.get();
}

void Buffer::free() {
  if (valid()) {
    // Destroy owned image views, image and free the memory.
    HandleGenerator<Buffer, BufferView>::destroyAllHandles();
    data_->vk_buffer.destroy();
    vmaFreeMemory(data_->allocator, data_->allocation);

    DestroyableOwnedHandle<AllocationManager>::free();
  }
}

} // namespace logi
