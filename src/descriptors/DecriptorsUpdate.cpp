#include "descriptors/DecriptorsUpdate.h"

namespace logi {

void DescriptorSetUpdate::writeImages(const DescriptorSet& set, uint32_t dst_binding, uint32_t dst_array_element,
    vk::DescriptorType descriptor_type, const std::vector<DescriptorImageInfo>& image_infos) {
    
	image_infos_.emplace_back();
	std::vector<vk::DescriptorImageInfo>& iinfos = image_infos_.back();
	iinfos.reserve(image_infos.size());

	// Generate Vulkan DescriptorImageInfos.
	for (const DescriptorImageInfo& info : image_infos) {
		iinfos.emplace_back(info.sampler.getVkHandle(), info.view.getVkHandle(), info.layout);
	}

	// Generate descriptor write.
	descriptor_writes_.emplace_back(set.getVkHandle(), dst_binding, dst_array_element, iinfos.size(), &iinfos, nullptr, nullptr);
}

void DescriptorSetUpdate::writeBuffers(const DescriptorSet& set, uint32_t dst_binding, uint32_t dst_array_element,
	vk::DescriptorType descriptor_type, const std::vector<DescriptorBufferInfo>& buffer_infos) {

	buffer_infos_.emplace_back();
	std::vector<vk::DescriptorBufferInfo>& binfos = buffer_infos_.back();
	binfos.reserve(buffer_infos.size());

	// Generate Vulkan DescriptorBufferInfos.
	for (const DescriptorBufferInfo& info : buffer_infos) {
		binfos.emplace_back(info.buffer.getVkHandle(), info.offset, info.range);
	}

	// Generate descriptor write.
	descriptor_writes_.emplace_back(set.getVkHandle(), dst_binding, dst_array_element, binfos.size(), nullptr, &binfos, nullptr);
}

void DescriptorSetUpdate::writeBufferView(const DescriptorSet& set, uint32_t dst_binding, uint32_t dst_array_element,
    vk::DescriptorType descriptor_type, const std::vector<BufferView>& buffer_views) {
    
	buffer_views_.emplace_back();
	std::vector<vk::BufferView>& bviews = buffer_views_.back();
	bviews.reserve(buffer_views_.size());

	// Store buffer views.
	for (const BufferView& view : buffer_views) {
		bviews.emplace_back(view.getVkHandle());
	}

	// Generate descriptor write.
	descriptor_writes_.emplace_back(set.getVkHandle(), dst_binding, dst_array_element, bviews.size(), nullptr, nullptr, &bviews);

}


}
