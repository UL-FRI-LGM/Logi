#ifndef MEMORY_ALLOCATION_MANAGER_H
#define MEMORY_ALLOCATION_MANAGER_H

#include "memory/MemoryUsage.h"
#include "memory/Image.h"
#include "memory/Buffer.h"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include <list>

namespace vkr {

class AllocationManager {
public:
	AllocationManager(const vk::PhysicalDevice& physical_device, const vk::Device& logical_device);

	Buffer* allocateBuffer(const BufferConfiguration& configuration);

	void freeBuffer(Buffer* buffer);

	Image* allocateImage(const ImageConfiguration& configuration);

	void freeImage( Image* image);

	~AllocationManager();

private:
	vk::Device device_;
	VmaAllocator allocator_;
	std::list<std::unique_ptr<Buffer>> allocated_buffers_;
	std::list<std::pair<VmaAllocation, std::unique_ptr<Image>>> allocated_images_;
};

}

#endif // !ALLOCATION_MANAGER
