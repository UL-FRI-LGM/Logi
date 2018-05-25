#include "memory/Image.h"

namespace vkr {

ImageConfiguration::ImageConfiguration(vk::ImageCreateFlags create_flags, vk::ImageType type, vk::Format format, vk::Extent3D extent, uint32_t mip_levels, uint32_t array_layers, vk::SampleCountFlagBits samples, vk::ImageTiling tiling, MemoryUsage memory_usage, bool concurrent, std::vector<uint32_t> concurrent_queue_families)
	: create_flags(create_flags), type(type), format(format), extent(extent), mip_levels(mip_levels), array_layers(array_layers), samples(samples), tiling(tiling), memory_usage(memory_usage), concurrent(concurrent), concurrent_queue_families(concurrent_queue_families) {
}

Image::Image(const vk::Device& device, VmaAllocator allocator, const VmaAllocation& allocation, const vk::Image& image, const ImageConfiguration& configuration) : device_(device), allocator_(allocator), allocation_(allocation), image_(image), configuration_(configuration) {
}

ImageView Image::createImageView(vk::Format format, const vk::ComponentMapping& component_mapping, const vk::ImageAspectFlags& aspect_mask, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_array_layer, uint32_t layer_count) {
	vk::ImageSubresourceRange subres_range{};
	subres_range.aspectMask = aspect_mask;
	subres_range.baseMipLevel = base_mip_level;
	subres_range.baseArrayLayer = base_array_layer;
	subres_range.layerCount = layer_count;

	vk::ImageViewCreateInfo image_view_ci{};
	image_view_ci.format = format;
	image_view_ci.components = component_mapping;
	image_view_ci.image = image_;
	image_view_ci.subresourceRange = subres_range;

	// Create ImageView resource.
	image_views_.emplace_back(std::make_shared<ManagedVkImageView>(device_, device_.createImageView(image_view_ci)));
	return ImageView(image_views_.back(), format, component_mapping, aspect_mask, base_mip_level, mip_level_count, base_array_layer, layer_count);
}


const vk::Image& Image::getVkImage() {
	return image_;
}

const VmaAllocation& Image::getAllocation() const {
	return allocation_;
}

Image::~Image() {
	image_views_.clear();
	device_.destroyImage(image_);
}

}