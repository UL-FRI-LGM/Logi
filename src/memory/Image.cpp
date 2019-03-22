#include "logi/memory/Image.h"
#include <utility>
#include "logi/base/LogicalDevice.h"
#include "logi/memory/AllocationManager.h"

namespace logi {

ImageConfiguration::ImageConfiguration(vk::ImageType type, vk::Format format, const vk::Extent3D& extent,
                                       uint32_t mip_levels, uint32_t array_layers, vk::SampleCountFlagBits samples,
                                       vk::ImageTiling tiling, const vk::ImageUsageFlags& usage,
                                       MemoryUsage memory_usage, vk::ImageLayout initial_layout,
                                       std::vector<uint32_t> concurrent_queue_families,
                                       const vk::ImageCreateFlags& create_flags)
  : create_flags(create_flags), type(type), format(format), extent(extent), mip_levels(mip_levels),
    array_layers(array_layers), samples(samples), tiling(tiling), usage(usage), memory_usage(memory_usage),
    initial_layout(initial_layout), concurrent_queue_families(std::move(concurrent_queue_families)) {}

Image::Image(const AllocationManager& alloc_manager, const vk::Image& image, VmaAllocator allocator,
             VmaAllocation allocation, const ImageConfiguration& configuration)
  : DestroyableOwnedHandle<Image, AllocationManager, Swapchain>(alloc_manager, true),
    data_(std::make_shared<Data>(allocator, allocation, configuration,
                                 ManagedVkImage(static_cast<vk::Device>(getOwner<LogicalDevice>()), image))) {}

Image::Image(const Swapchain& swap_chain, const vk::Image& image)
  : DestroyableOwnedHandle<Image, AllocationManager, Swapchain>(swap_chain, true),
    data_(std::make_shared<Data>(nullptr, nullptr, ImageConfiguration(),
                                 ManagedVkImage(static_cast<vk::Device>(getOwner<LogicalDevice>()), image))) {}

ImageView Image::createImageView(const ImageViewConfiguration& configuration) const {
  checkForNullHandleInvocation("Image", "createImageView");

  // Create and register handle.
  return HandleGenerator<Image, ImageView>::createHandle(configuration);
}

const vk::Image& Image::getVkHandle() const {
  checkForNullHandleInvocation("Image", "getVkHandle");
  return data_->vk_image.get();
}

Image::operator vk::Image() const {
  checkForNullHandleInvocation("Image", "operator vk::Image()");
  return data_->vk_image.get();
}

void Image::destroy() {
  checkForNullHandleInvocation("Image", "destroy");
  if (data_->allocator == nullptr) {
    throw IllegalInvocation("Tried to destroy swapchain image.");
  }

  DestroyableOwnedHandle<Image, AllocationManager, Swapchain>::destroy();
}

void Image::free() {
  if (valid()) {
    // Destroy owned image views, image and free the memory.
    HandleGenerator<Image, ImageView>::destroyAllHandles();

    // If the allocator is nullptr do not destroy image as it is a Swapchain image.
    if (data_->allocator != nullptr) {
      data_->vk_image.destroy();
      vmaFreeMemory(data_->allocator, data_->allocation);
    }

    DestroyableOwnedHandle<AllocationManager, Swapchain>::free();
  }
}

} // namespace logi
