#include "memory/Image.h"

namespace vkr {

ImageConfiguration::ImageConfiguration(vk::ImageCreateFlags create_flags, vk::ImageType type, vk::Format format, vk::Extent3D extent, uint32_t mip_levels, uint32_t array_layers, vk::SampleCountFlagBits samples, vk::ImageTiling tiling, MemoryUsage memory_usage, bool concurrent, std::vector<uint32_t> concurrent_queue_families)
	: create_flags(create_flags), type(type), format(format), extent(extent), mip_levels(mip_levels), array_layers(array_layers), samples(samples), tiling(tiling), memory_usage(memory_usage), concurrent(concurrent), concurrent_queue_families(concurrent_queue_families) {
}


ImageView::ImageView(const vk::Device& device, const vk::Image& image, vk::Format format, const vk::ComponentMapping& component_mapping, const vk::ImageAspectFlags& aspect_mask, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_array_layer, uint32_t layer_count)
	: device_(device), format_(format), component_mapping_(component_mapping), aspect_mask_(aspect_mask), base_mip_level_(base_mip_level), mip_level_count_(mip_level_count), base_array_layer_(base_array_layer), layer_count_(layer_count) {

	vk::ImageSubresourceRange subres_range{};
	subres_range.aspectMask = aspect_mask_;
	subres_range.baseMipLevel = base_mip_level_;
	subres_range.baseArrayLayer = base_array_layer_;
	subres_range.layerCount = layer_count_;

	vk::ImageViewCreateInfo image_view_ci{};
	image_view_ci.format = format_;
	image_view_ci.components = component_mapping;
	image_view_ci.image = image;
	image_view_ci.subresourceRange = subres_range;

	image_view_ = device_.createImageView(image_view_ci);
}

const vk::ImageView& ImageView::getVkImageView() {
	return image_view_;
}

ImageView::~ImageView() {
	device_.destroyImageView(image_view_);
}


Image::Image(const vk::Device& device, const vk::Image& image, const ImageConfiguration& configuration) : device_(device), image_(image), configuration_(configuration) {
}

ImageView* Image::createImageView(vk::Format format, const vk::ComponentMapping& component_mapping, const vk::ImageAspectFlags& aspect_mask, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_array_layer, uint32_t layer_count) {
	// TODO: Validation
	image_views_.emplace_back(std::make_unique<ImageView>(device_, image_, format, component_mapping, aspect_mask, base_mip_level, mip_level_count, base_array_layer, layer_count));
	return image_views_.back().get();
}

void Image::destroyImageView(ImageView* image_view) {
	auto it = std::find_if(image_views_.begin(), image_views_.end(), [&image_view](const std::unique_ptr<ImageView>& entry) {
		return entry.get() == image_view;
	});

	// Destroy buffer view.
	if (it != image_views_.end()) {
		image_views_.erase(it);
	}
}


const vk::Image& Image::getVkImage() {
	return image_;
}

Image::~Image() {
	image_views_.clear();
	device_.destroyImage(image_);
}

}