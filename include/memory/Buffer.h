#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <list>
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

/**
 * @brief Buffer view wrapper.
 */
class BufferView {
public:
	/**
	 * @brief Create new BufferView.
	 *
	 * @param device Vulkan logical device handle.
	 * @param buffer Referenced buffer.
	 * @param format Buffer view element format.
	 * @parma offset Buffer view offset from the start of the buffer in Bytes.
	 * @param range Buffer view size (number of captured bytes from offset).
	 */
	BufferView(const vk::Device& device, const vk::Buffer& buffer, vk::Format format, vk::DeviceSize offset, vk::DeviceSize range);

	/**
	 * @brief Retrieve Vulkan BufferView handle.
	 *
	 * @return Vulkan BufferView handle.
	 */
	const vk::BufferView& getVkBufferView();

	/**
	 * @brief Free resources.
	 */
	~BufferView();

private:
	vk::Device device_;				///< Vulkan logical device handle.
	vk::BufferView buffer_view_;	///< Buffer view Vulkan handle.
	vk::Format format_;				///< Buffer view element format.
	vk::DeviceSize offset_;			///< Buffer view offset from the start of the buffer in Bytes.
	vk::DeviceSize range_;			///< Buffer view size (number of captured bytes from offset).
};


class Buffer {
public:
	/**
	 * @brief Wrap the given buffer and its configuration.
	 */
	Buffer(const vk::Device& device, VmaAllocator allocator, const VmaAllocation& allocation, const vk::Buffer& buffer, const BufferConfiguration& configuration);

	/**
	 * @brief Create buffer view for this buffer. Buffer must be texel based.
	 *
	 * @param format Buffer view element format.
	 * @param offset Buffer view offset from the start of the buffer in Bytes.
	 * @param range Buffer view size (number of captured bytes from offset).
	 */
	BufferView* createBufferView(vk::Format format, vk::DeviceSize offset, vk::DeviceSize range);

	/**
	 * @brief Destroy the given buffer view.
	 *
	 * @param buffer_view Buffer view that will be destroyed..
	 */
	void destroyBufferView(BufferView* buffer_view);

	std::vector<unsigned char> getData();

	/**
	 * @brief Get Vulkan buffer handle.
	 *
	 * @return Vulkan buffer handle.
	 */
	const vk::Buffer& getVkBuffer() const;

	const VmaAllocation& getAllocation() const;

	/**
	 * @brief Retrieve buffer configuration.
	 *
	 * @return Buffer configuration.
	 */
	const BufferConfiguration& getBufferConfiguration() const;

	/**
	 * @brief Frees resources.
	 */
	~Buffer();

private:
	vk::Device device_; ///< Vulkan logical device handle.
	VmaAllocator allocator_;
	VmaAllocation allocation_;
	vk::Buffer buffer_; ///< Vulkan buffer handle.
	
	
	BufferConfiguration configuration_; ///< Vulkan buffer configuration.
	std::list<std::unique_ptr<BufferView>> buffer_views_; ///< Allocated buffer views..
};

}

#endif // !MEMORY_BUFFER_H
