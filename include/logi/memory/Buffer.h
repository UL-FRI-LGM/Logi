#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include "logi/base/ManagedResource.h"
#include "logi/memory/BufferView.h"
#include "logi/memory/MemoryUsage.h"

namespace logi {

/**
 * @brief	Contains configuration for the Buffer.
 */
struct BufferConfiguration {
  /**
   * @brief	Default constructor. Initializes values to defaults.
   *
   * @param	size						Size in bytes of the buffer.
   * @param	usage						Intended usage of the image.
   * @param	concurrent_queue_families	Vector of QueueFamily indices. These QueueFamilies will be allowed to concurrently
   * use the image.
   * @param	flags						A bitmask of vk::BufferCreateFlagBits describing additional parameters of the buffer.
   */
  explicit BufferConfiguration(vk::DeviceSize size = 0, const vk::BufferUsageFlags& usage = vk::BufferUsageFlags(),
                               MemoryUsage memory_usage = MemoryUsage::GPU_ONLY,
                               std::vector<uint32_t> concurrent_queue_families = {},
                               const vk::BufferCreateFlags& flags = vk::BufferCreateFlags());

  /**
   * A bit mask of vk::BufferCreateFlagBits describing additional parameters of the buffer.
   */
  vk::BufferCreateFlags flags;

  /**
   * Size in bytes of the buffer.
   */
  vk::DeviceSize size;

  /**
   * Intended usage of the image.
   */
  vk::BufferUsageFlags usage;

  /**
   * Specifies who can use the allocated buffer (CPU/GPU).
   */
  MemoryUsage memory_usage;

  /**
   * Vector of QueueFamily indices. These QueueFamilies will be allowed to concurrently use the image.
   */
  std::vector<uint32_t> concurrent_queue_families;
};

class AllocationManager;

/**
 * @brief	Handle used to access Vulkan Buffer resource.
 */
class Buffer : public DestroyableOwnedHandle<AllocationManager>, public HandleGenerator<Buffer, BufferView> {
 public:
  /**
   * @brief Default placeholder constructor.
   */
  Buffer() = default;

  /**
   * @brief	Create handle used to access Vulkan Buffer resource.
   *
   * @param	alloc_manager Device allocation manager.
   * @param	buffer        Vulkan buffer handle.
   * @param	allocator     Pointer to the Vulkan memory allocator.
   * @param	allocation    Pointer to the structure containing the allocation meta data.
   * @param	configuration Buffer configuration.
   */
  Buffer(const AllocationManager& alloc_manager, const vk::Buffer& buffer, VmaAllocator allocator,
         VmaAllocation allocation, const BufferConfiguration& configuration);

  /**
   * @brief	Create BufferView for this Buffer.
   *
   * @param	configuration	ImageView configuration.
   * @return	BufferView handle.
   */
  BufferView createBufferView(const BufferViewConfiguration& configuration) const;

  // std::vector<unsigned char> getData();

  // void writeData(void* data, size_t size_bytes);

  /**
   * @brief Get Vulkan buffer handle.
   *
   * @return Vulkan buffer handle.
   */
  const vk::Buffer& getVkHandle() const;

  void writeData(void* data, size_t size_bytes);

  /**
   * @brief   Conversion to vk::Buffer.
   *
   * @return  vk::Buffer.
   */
  operator vk::Buffer() const;

 protected:
  /**
   * @brief	Destroys owned BufferView resoucres, destroys Vulkan Buffer handle and frees the memory.
   */
  void free() override;

 private:
  using ManagedVkBuffer = ManagedResource<vk::Device, vk::Buffer, vk::DispatchLoaderStatic, &vk::Device::destroyBuffer>;

  /**
   * @brief Buffer internal data.
   */
  struct Data {
    /**
     * @brief Initialize buffer internal data.
     *
     * @param allocator   Allocator pointer.
     * @param allocation  Allocation pointer.
     * @param config      Image configuration.
     * @param vk_buffer   Vulkan buffer handle.
     */
    Data(VmaAllocator allocator, VmaAllocation allocation, BufferConfiguration config, ManagedVkBuffer vk_buffer)
      : allocator(allocator), allocation(allocation), config(std::move(config)), vk_buffer(vk_buffer) {}

    /**
     * Allocator pointer.
     */
    VmaAllocator allocator;

    /**
     * Allocation pointer.
     */
    VmaAllocation allocation;

    /**
     * Buffer configuration.
     */
    BufferConfiguration config;

    /**
     * Vulkan buffer handle.
     */
    ManagedVkBuffer vk_buffer;
  };

  /**
   * Buffer internal data.
   */
  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif // !MEMORY_BUFFER_H
