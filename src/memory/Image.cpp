#include "..\..\include\memory\Image.h"

vkr::ImageConfiguration::ImageConfiguration(vk::ImageCreateFlags create_flags, vk::ImageType type, vk::Format format, vk::Extent3D extent, uint32_t mip_levels, uint32_t array_layers, vk::SampleCountFlagBits samples, vk::ImageTiling tiling, MemoryUsage memory_usage, bool concurrent, std::vector<uint32_t> concurrent_queue_families)
	: create_flags(create_flags), type(type), format(format), extent(extent), mip_levels(mip_levels), array_layers(array_layers), samples(samples), tiling(tiling), memory_usage(memory_usage), concurrent(concurrent), concurrent_queue_families(concurrent_queue_families) { }

vkr::Image::Image(const vk::Image& image, const VmaAllocation& allocation, const ImageConfiguration& configuration) : image_(image), allocation_(allocation), configuration_(configuration){
}

const vk::Image& vkr::Image::getVkImage() {
	return image_;
}

const VmaAllocation& vkr::Image::getAllocation() {
	return allocation_;
}
