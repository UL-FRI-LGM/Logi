/*
* Descriptor pool
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef DESCRIPTORS_DESCRIPTORS_UPDATE_H
#define DESCRIPTORS_DESCRIPTORS_UPDATE_H

#include <utility>
#include <vulkan/vulkan.hpp>
#include "descriptors/DescriptorPool.h"
#include "memory/Buffer.h"
#include "memory/ImageView.h"
#include "memory/Image.h"
#include "memory/Sampler.h"

namespace logi {


struct DescriptorBufferInfo {
    DescriptorBufferInfo(Buffer buffer, const uint64_t offset, const uint64_t range)
        : buffer(std::move(buffer)),
          offset(offset),
          range(range) {}

	Buffer buffer;
	uint64_t offset;
	uint64_t range;
};

struct DescriptorImageInfo {
    DescriptorImageInfo(Sampler sampler, ImageView view, const vk::ImageLayout layout)
        : sampler(std::move(sampler)),
          view(std::move(view)),
          layout(layout) {}

	Sampler sampler;
	ImageView view;
	vk::ImageLayout layout;
};


class DescriptorSetUpdate : public Handle {
public:
	void writeImages(const DescriptorSet& set, uint32_t dst_binding, uint32_t dst_array_element,
		vk::DescriptorType descriptor_type, const std::vector<DescriptorImageInfo>& image_infos);

	void writeBuffers(const DescriptorSet& set, uint32_t dst_binding, uint32_t dst_array_element,
		vk::DescriptorType descriptor_type, const std::vector<DescriptorBufferInfo>& buffer_infos);

	void writeBufferView(const DescriptorSet& set, uint32_t dst_binding, uint32_t dst_array_element,
		vk::DescriptorType descriptor_type, const std::vector<BufferView>& buffer_views);

private:
	std::vector<vk::WriteDescriptorSet> descriptor_writes_;

	std::list<std::vector<std::unique_ptr<ExtensionObject>>> extensions_;
	std::list<std::vector<vk::DescriptorBufferInfo>> buffer_infos_;
	std::list<std::vector<vk::DescriptorImageInfo>> image_infos_;
	std::list<std::vector<vk::BufferView>> buffer_views_;
};


}


#endif
