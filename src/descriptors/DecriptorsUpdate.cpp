#include "descriptors/DecriptorsUpdate.h"
#include "base/Exceptions.h"

namespace logi {

vk::WriteDescriptorSet DescriptorWrite::build() {
	return { set.getVkHandle(), dst_binding, dst_array_element, 0, set.getLayout().getDescriptorBinding(dst_binding).type,
			 nullptr, nullptr, nullptr };
}

vk::WriteDescriptorSet DescriptorImageWrite::build() {
    // Build Vulkan image infos.
	vk_image_infos_.reserve(image_infos.size());
    for (const DescriptorImageInfo& info : image_infos) {
		vk_image_infos_.emplace_back(info.build());
    }

    // Add image info data.
	vk::WriteDescriptorSet vk_write = DescriptorWrite::build();
	vk_write.descriptorCount = vk_image_infos_.size();
	vk_write.pImageInfo = vk_image_infos_.data();
	vk_write.pNext = buildExtensions();

	return vk_write;
}

vk::WriteDescriptorSet DescriptorBufferWrite::build() {
	// Build Vulkan buffer infos.
	vk_buffer_infos_.reserve(buffer_infos.size());
	for (const DescriptorBufferInfo& info : buffer_infos) {
		vk_buffer_infos_.emplace_back(info.build());
	}

	// Add image info data.
	vk::WriteDescriptorSet vk_write = DescriptorWrite::build();
	vk_write.descriptorCount = vk_buffer_infos_.size();
	vk_write.pBufferInfo = vk_buffer_infos_.data();

	return vk_write;
}

vk::WriteDescriptorSet DescriptorBufferViewWrite::build() {
	// Collect Vulkan buffer views.
	vk_buffer_views_.reserve(buffer_views.size());
	for (const BufferView& view : buffer_views) {
		vk_buffer_views_.emplace_back(view.getVkHandle());
	}

	// Add image info data.
	vk::WriteDescriptorSet vk_write = DescriptorWrite::build();
	vk_write.descriptorCount = vk_buffer_views_.size();
	vk_write.pTexelBufferView = vk_buffer_views_.data();

	return vk_write;
}

vk::CopyDescriptorSet DescriptorSetCopy::build() const {
	return { src_set.getVkHandle(), src_binding, src_array_element, dst_set.getVkHandle(), dst_binding,
	         dst_array_element, descriptor_count };
}

DescriptorSetUpdate::DescriptorSetUpdate()
	: DependentDestroyableHandle({}, false) {}

DescriptorSetUpdate::DescriptorSetUpdate(std::weak_ptr<HandleManager>& owner, const vk::Device& device)
    : DependentDestroyableHandle(owner), data_(std::make_shared<DescriptorSetUpdateData>(device)) {}

void DescriptorSetUpdate::write(const DescriptorImageWrite& image_write) const {
	checkForNullHandleInvocation("DescriptorSetUpdate", "write");

	data_->image_writes.emplace_back(image_write);
	data_->writes.emplace_back(data_->image_writes.back().build());
}

void DescriptorSetUpdate::write(const DescriptorBufferWrite& buffer_write) const {
	checkForNullHandleInvocation("DescriptorSetUpdate", "write");

	data_->buffer_writes.emplace_back(buffer_write);
	data_->writes.emplace_back(data_->buffer_writes.back().build());
}

void DescriptorSetUpdate::write(const DescriptorBufferViewWrite& buffer_view_write) const {
	checkForNullHandleInvocation("DescriptorSetUpdate", "write");

	data_->buffer_view_writes.emplace_back(buffer_view_write);
	data_->writes.emplace_back(data_->buffer_view_writes.back().build());
}

void DescriptorSetUpdate::copy(const DescriptorSetCopy& set_copy) const {
	checkForNullHandleInvocation("DescriptorSetUpdate", "copy");

	data_->copies.emplace_back(set_copy.build());
}

void DescriptorSetUpdate::execute() const {
	checkForNullHandleInvocation("DescriptorSetUpdate", "update");

	data_->device.updateDescriptorSets(data_->writes, data_->copies);
}

DescriptorSetUpdate::DescriptorSetUpdateData::DescriptorSetUpdateData(const vk::Device device)
    : device(device) {}

}
