#ifndef MEMORY_ALLOCATION_MANAGER_H
#define MEMORY_ALLOCATION_MANAGER_H

#include "memory/MemoryUsage.h"
#include "memory/Image.h"
#include "memory/Buffer.h"

#include <vulkan/vulkan.hpp>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <list>

namespace vkr {

class AllocationManager {
public:
	AllocationManager(const vk::PhysicalDevice& physical_device, const vk::Device& logical_device);

	std::shared_ptr<Buffer> allocateBuffer(const BufferConfiguration& configuration);

	void freeBuffer(const std::shared_ptr<Buffer>& buffer);

	std::shared_ptr<Image> allocateImage(const ImageConfiguration& configuration);

	void freeImage(const std::shared_ptr<Image>& image);

	~AllocationManager();

private:
	VmaAllocator allocator_;
	std::list<std::shared_ptr<Buffer>> allocated_buffers_;
	std::list<std::shared_ptr<Image>> allocated_images_;
};

}

#endif // !ALLOCATION_MANAGER
