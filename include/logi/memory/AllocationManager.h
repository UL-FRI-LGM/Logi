#ifndef MEMORY_ALLOCATION_MANAGER_H
#define MEMORY_ALLOCATION_MANAGER_H

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/memory/Buffer.h"
#include "logi/memory/Image.h"

namespace logi {

class LogicalDevice;

/**
 * @brief	Allocation manager Handle used to allocate Image and Buffer resources.
 */
class AllocationManager : public OwnedHandle<LogicalDevice>,
                          public HandleGenerator<AllocationManager, Buffer>,
                          public HandleGenerator<AllocationManager, Image> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  AllocationManager() = default;

  /**
   * @brief	Create AllocationManager and initialize allocator.
   *
   * @param	device	Logical device handle.
   */
  AllocationManager(const LogicalDevice& device);

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
  VmaAllocator allocator_; ///< Vulkan memory allocator handle.
};

} // namespace logi

#endif // !ALLOCATION_MANAGER
