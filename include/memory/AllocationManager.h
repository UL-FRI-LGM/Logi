#ifndef MEMORY_ALLOCATION_MANAGER_H
#define MEMORY_ALLOCATION_MANAGER_H

#include "base/Handle.h"
#include "memory/Image.h"
#include "memory/Buffer.h"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>


namespace logi {

/**
 * @brief	Allocation manager Handle used to allocate Image and Buffer resources.
 */
class AllocationManager : public Handle {
public:
	/**
	 * @brief	Default placeholder constructor.
	 */
	AllocationManager();

	/**
	 * @brief	Create AllocationManager and intialize allocator.
	 *
	 * @param	physical_device	Vulkan physical device handle.
	 * @param	logical_device	Vulkan logical device handle.
	 */
	AllocationManager(const vk::PhysicalDevice& physical_device, const vk::Device& logical_device);

	/**
	 * @brief	Allocates memory for new Buffer, initializes it with the given configuration and returns a Buffer handle.
	 *
	 * @param	configuration	Buffer configuration.
	 * @return	Buffer handle.
	 */
	Buffer createBuffer(const BufferConfiguration& configuration);

	/**
	 * @brief	Allocates memory for new Image, initializes it with the given configuration and returns a Image handle.
	 *
	 * @param	configuration	Image configuration.
	 * @return	Image handle.
	 */
	Image createImage(const ImageConfiguration& configuration);

protected:
	/**
	 * @brief	Frees allocator and all resources allocated from it.
	 */
	void free() override;

private:
	vk::Device vk_device_;									///< Vulkan device handle.
	VmaAllocator allocator_;								///< Vulkan memory allocator handle.
	std::shared_ptr<HandleManager> allocation_handles_;		///< HandleManager that manages Image and Buffer handles.
};

}

#endif // !ALLOCATION_MANAGER
