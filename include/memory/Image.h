#ifndef MEMORY_IMAGE_H
#define MEMORY_IMAGE_H

#include <vulkan/vulkan.hpp>
#include "memory/MemoryUsage.h"
#include <vk_mem_alloc.h>

namespace vkr {

struct ImageConfiguration {
	vk::ImageCreateFlags create_flags;
	vk::ImageType type;
	vk::Format format;
	vk::Extent3D extent;
	uint32_t mip_levels;
	uint32_t array_layers;
	vk::SampleCountFlagBits samples;
	vk::ImageTiling tiling;
	vk::ImageLayout initial_layout;
	MemoryUsage memory_usage;
	bool concurrent;
	std::vector<uint32_t> concurrent_queue_families;

	ImageConfiguration(vk::ImageCreateFlags create_flags = vk::ImageCreateFlags(), vk::ImageType type = vk::ImageType::e1D, vk::Format format = vk::Format::eUndefined, vk::Extent3D extent = vk::Extent3D(), uint32_t mip_levels = 0, uint32_t array_layers = 0,
									vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1, vk::ImageTiling tiling = vk::ImageTiling::eOptimal, MemoryUsage memory_usage = MemoryUsage::GPU_ONLY, bool concurrent = false, std::vector<uint32_t> concurrent_queue_families = {});
};


class Image {
public:
	Image(const vk::Image& image, const VmaAllocation& allocation, const ImageConfiguration& configuration);

	const vk::Image& getVkImage();

	const VmaAllocation& getAllocation();

private:
	vk::Image image_;
	VmaAllocation allocation_;
	ImageConfiguration configuration_;
};

}

#endif // !MEMORY_BUFFER_H
