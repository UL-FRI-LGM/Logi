#ifndef MEMORY_IMAGE_H
#define MEMORY_IMAGE_H

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include "logi/base/ManagedResource.h"
#include "logi/memory/ImageView.h"
#include "logi/memory/MemoryUsage.h"

namespace logi {

/**
 * @brief	Contains configuration for the Image.
 */
struct ImageConfiguration {
  /**
   * @brief	Default constructor. Initializes values to defaults.
   *
   * @param	type						Specifies the basic dimensionality of the image.
   * @param	format						Describes the format and type of the data elements that will be contained in the image.
   * @param	extent						Describes the number of data elements in each dimension of the base level.
   * @param	mip_levels					Number of levels of detail available for minified sampling of the image.
   * @param	array_layers				Number of layers in the image.
   * @param	samples						Number of sub-data element samples in the image.
   * @param	tiling						Specifies the tiling arrangement of the data elements in memory.
   * @param	usage						Intended usage of the image.
   * @param	memory_usage				Specifies who can use the allocated image.
   * @param	initial_layout				Specifying the initial vk::ImageLayout of all image subresources of the image
   * @param	concurrent_queue_families	Vector of QueueFamily indices. These QueueFamilies will be allowed to concurrently
   * use the image.
   * @param	create_flags				A bitmask of vk::ImageCreateFlagBits describing additional parameters of the image.
   */
  explicit ImageConfiguration(vk::ImageType type = vk::ImageType::e1D, vk::Format format = vk::Format::eUndefined,
                              const vk::Extent3D& extent = vk::Extent3D(), uint32_t mip_levels = 0,
                              uint32_t array_layers = 0, vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1,
                              vk::ImageTiling tiling = vk::ImageTiling::eOptimal,
                              const vk::ImageUsageFlags& usage = vk::ImageUsageFlags(),
                              MemoryUsage memory_usage = MemoryUsage::GPU_ONLY,
                              vk::ImageLayout initial_layout = vk::ImageLayout::eGeneral,
                              std::vector<uint32_t> concurrent_queue_families = {},
                              const vk::ImageCreateFlags& create_flags = vk::ImageCreateFlags());

  vk::ImageCreateFlags
    create_flags;        ///< A bitmask of vk::ImageCreateFlagBits describing additional parameters of the image.
  vk::ImageType type;    ///< Specifies the basic dimensionality of the image.
  vk::Format format;     ///< Describes the format and type of the data elements that will be contained in the image.
  vk::Extent3D extent;   ///< Describes the number of data elements in each dimension of the base level.
  uint32_t mip_levels;   ///< Number of levels of detail available for minified sampling of the image.
  uint32_t array_layers; ///< Number of layers in the image.
  vk::SampleCountFlagBits samples; ///< Number of sub-data element samples in the image.
  vk::ImageTiling tiling;          ///< Specifies the tiling arrangement of the data elements in memory.
  vk::ImageUsageFlags usage;       ///< Describes the intended usage of the image.
  MemoryUsage memory_usage;        ///< Specifies who can use the allocated image (CPU/GPU).
  vk::ImageLayout initial_layout;  ///< Specifying the initial vk::ImageLayout of all image subresources of the image
  std::vector<uint32_t> concurrent_queue_families; ///< Vector of QueueFamily indices. These QueueFamilies will be
                                                   ///< allowed to concurrently use the image.
};

class AllocationManager;
class Swapchain;

/**
 * @brief	Handle used to access Vulkan Image resource.
 */
class Image : public DestroyableOwnedHandle<Image, AllocationManager, Swapchain>,
              public HandleGenerator<Image, ImageView> {
 public:
  /**
   * @brief Default placeholder constructor.
   */
  Image() = default;

  /**
   * @brief	Create handle used to access Vulkan Image resource.
   *
   * @param	alloc_manager Device allocation manager.
   * @param	image			    Vulkan image handle.
   * @param	allocator		  Pointer to the Vulkan memory allocator.
   * @param	allocation		Pointer to the structure containing the allocation meta data.
   * @param	configuration	Image configuration.
   */
  Image(const AllocationManager& alloc_manager, const vk::Image& image, VmaAllocator allocator,
        VmaAllocation allocation, const ImageConfiguration& configuration);

  /**
   * @brief	Create handle used to access Vulkan Image resource.
   *
   * @param	swap_chain    Owner swapchain.
   * @param	image			    Vulkan image handle.
   * @param	configuration	Image configuration.
   */
  Image(const Swapchain& swap_chain, const vk::Image& image);

  /**
   * @brief	Create ImageView for this Image.
   *
   * @param	configuration	ImageView configuration.
   * @return	ImageView handle.
   */
  ImageView createImageView(const ImageViewConfiguration& configuration) const;

  /**
   * @brief	Retrieve Vulkan Image handle.
   *
   * @return	Reference to Vulkan Image handle.
   */
  const vk::Image& getVkHandle() const;

  /**
   * @brief   vk::Image conversion.
   *
   * @return	vk::Image
   */
  operator vk::Image() const;

  void destroy() override;

 protected:
  /**
   * @brief	Destroys owned ImageView resources, destroys Vulkan Image handle and frees the memory.
   */
  void free() override;

 private:
  using ManagedVkImage = ManagedResource<vk::Device, vk::Image, vk::DispatchLoaderStatic, &vk::Device::destroyImage>;

  struct Data {
    /**
     * @brief Initialize image internal data.
     *
     * @param allocator   Allocator pointer.
     * @param allocation  Allocation pointer.
     * @param config      Image configuration.
     * @param vk_image    Vulkan image handle.
     */
    Data(VmaAllocator allocator, VmaAllocation allocation, ImageConfiguration config, ManagedVkImage vk_image)
      : allocator(allocator), allocation(allocation), config(std::move(config)), vk_image(vk_image) {}

    /**
     * Allocator pointer.
     */
    VmaAllocator allocator;

    /**
     * Allocation pointer.
     */
    VmaAllocation allocation;

    /**
     * Image configuration.
     */
    ImageConfiguration config;

    /**
     * Vulkan image handle.
     */
    ManagedVkImage vk_image;
  };

  /**
   * Image internal data.
   */
  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif // !MEMORY_BUFFER_H
