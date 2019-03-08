#include "logi/memory/Image.h"

namespace logi {


ImageConfiguration::ImageConfiguration(vk::ImageType type, vk::Format format, const vk::Extent3D& extent, uint32_t mip_levels,
	uint32_t array_layers, vk::SampleCountFlagBits samples, vk::ImageTiling tiling, const vk::ImageUsageFlags& usage,
	 MemoryUsage memory_usage, vk::ImageLayout initial_layout, const std::vector<uint32_t>& concurrent_queue_families,
	 const vk::ImageCreateFlags& create_flags)
	: create_flags(create_flags), type(type), format(format), extent(extent), mip_levels(mip_levels), array_layers(array_layers),
	  samples(samples), tiling(tiling), usage(usage), memory_usage(memory_usage), initial_layout(initial_layout), concurrent_queue_families(concurrent_queue_families) {}

Image::Image()
	: DependentDestroyableHandle({}, false), allocator_(nullptr), allocation_(nullptr) {
}

Image::Image(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Image& image, VmaAllocator allocator, VmaAllocation allocation, const ImageConfiguration& configuration)
	: DependentDestroyableHandle(owner), allocator_(allocator), allocation_(allocation), configuration_(std::make_shared<ImageConfiguration>(configuration)),
	  vk_image_(std::make_shared<ManagedVkImage>(device, image)), image_view_hm_(std::make_shared<HandleManager>()) { }


ImageView Image::createImageView(const ImageViewConfiguration& configuration) const {
	if (!alive()) {
		throw std::runtime_error("Called 'createImageView' on destroyed Image object.");
	}

	// Create and register handle.
	return image_view_hm_->createHandle<ImageView>(vk_image_->getOwner(), vk_image_->get(), configuration);
}

const vk::Image& Image::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called 'getVkHandle' on destroyed Image object.");
	}

	return vk_image_->get();
}

void Image::free() {
	if (alive()) {
		// Destroy owned image views, image and free the memory.
		image_view_hm_->destroyAllHandles();
		vk_image_->destroy();
		vmaFreeMemory(allocator_, allocation_);

		Handle::free();
	}
}

}
