#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include "base/ManagedResource.h"
#include "memory/BufferView.h"
#include "memory/MemoryUsage.h"

namespace logi {

/**
 * @brief	Contains configuration for the Buffer.
 */
struct BufferConfiguration {
	/**
	 * @brief	Default constructor. Initializes values to defaults.
	 *
	 * @param	flags						A bitmask of vk::BufferCreateFlagBits describing additional parameters of the buffer.
	 * @param	size						Size in bytes of the buffer.
	 * @param	usage						Intended usage of the image.
	 * @param	concurrent_queue_families	Vector of QueueFamily indices. These QueueFamilies will be allowed to concurrently use the image.
	 */
	explicit BufferConfiguration(vk::BufferCreateFlags flags = vk::BufferCreateFlags(), vk::DeviceSize size = 0, vk::BufferUsageFlags usage = vk::BufferUsageFlags(), MemoryUsage memory_usage = MemoryUsage::GPU_ONLY, std::vector<uint32_t> concurrent_queue_families = {});

	vk::BufferCreateFlags flags;						///< A bitmask of vk::BufferCreateFlagBits describing additional parameters of the buffer.
	vk::DeviceSize size;								///< Size in bytes of the buffer.
	vk::BufferUsageFlags usage;							///< Intended usage of the image.
	MemoryUsage memory_usage;							///< Specifies who can use the allocated buffer (CPU/GPU).
	std::vector<uint32_t> concurrent_queue_families;	///< Vector of QueueFamily indices. These QueueFamilies will be allowed to concurrently use the image.
};

/**
 * @brief	Handle used to access Vulkan Buffer resource.
 */
class Buffer : public DependentDestroyableHandle {
public:
	/**
	 * @brief	Create handle used to access Vulkan Buffer resource.
	 *
	 * @param	owner			Pointer to a HandleManager responsible for this handle.
	 * @param	device			Device to which the buffer belongs.
	 * @param	buffer			Vulkan buffer handle.
	 * @param	allocator		Pointer to the Vulkan memory allocator.
	 * @param	allocation		Pointer to the structure containing the allocation meta data.
	 * @param	configuration	Buffer configuration.
	 */
	Buffer(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Buffer& buffer, VmaAllocator allocator, VmaAllocation allocation, const BufferConfiguration& configuration);

	/**
	 * @brief	Create BufferView for this Buffer.
	 *
	 * @param	configuration	ImageView configuration.
	 * @return	BufferView handle.
	 */
	BufferView createBufferView(const BufferViewConfiguration& configuration) const;

	//std::vector<unsigned char> getData();

	//void writeData(void* data, size_t size_bytes);

	/**
	 * @brief Get Vulkan buffer handle.
	 *
	 * @return Vulkan buffer handle.
	 */
	const vk::Buffer& getVkHandle() const;

protected:
	/**
	 * @brief	Destroys owned BufferView resoucres, destroys Vulkan Buffer handle and frees the memory.
	 */
	void free() override;

private:
	using ManagedVkBuffer = ManagedResource<vk::Device, vk::Buffer, vk::DispatchLoaderStatic, &vk::Device::destroyBuffer>;

	VmaAllocator allocator_;								///< Pointer to the responsible allocator.
	VmaAllocation allocation_;								///< Pointer to the allocation.
	std::shared_ptr<BufferConfiguration> configuration_;	///< Buffer configuration.
	std::shared_ptr<ManagedVkBuffer> vk_buffer_;			///< Vulkan buffer handle.
	std::shared_ptr<HandleManager> buffer_view_hm_;			///< Buffer view handle manager.
};

}

#endif // !MEMORY_BUFFER_H
