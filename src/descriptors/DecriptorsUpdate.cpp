#include "descriptors/DecriptorsUpdate.h"

namespace logi {
logi::DescriptorsUpdate::DescriptorsUpdate() : write_operations_(), copy_operations_(), buffer_infos_(), image_infos_() {
}

void DescriptorsUpdate::copyDescriptorSet(const DescriptorSet& src_set, uint32_t src_binding, uint32_t src_array_element, const DescriptorSet& dst_set, uint32_t dst_binding, uint32_t dst_array_element, uint32_t descriptor_count) {
	vk::CopyDescriptorSet copy_ds{};
	copy_ds.srcSet = src_set.getVkHandle();
	copy_ds.srcBinding = src_binding;
	copy_ds.srcArrayElement = src_array_element;
	copy_ds.dstSet = dst_set.getVkHandle();
	copy_ds.dstBinding = dst_binding;
	copy_ds.dstArrayElement = dst_array_element;
	copy_ds.descriptorCount = descriptor_count;

	copy_operations_.push_back(copy_ds);
}


void DescriptorsUpdate::writeBufferToDescriptorSet(const DescriptorSet& dst_set, uint32_t dst_binding, uint32_t dst_array_element, const Buffer& buffer, vk::DeviceSize offset, vk::DeviceSize range) {
	buffer_infos_.emplace_back(std::make_unique<vk::DescriptorBufferInfo>(buffer.getVkHandle(), offset, range));

	vk::WriteDescriptorSet write_ds{};
	write_ds.descriptorCount = 1u;
	write_ds.dstSet = dst_set.getVkHandle();
	write_ds.dstBinding = dst_binding;
	write_ds.dstArrayElement = dst_array_element;
	write_ds.descriptorType = dst_set.getLayout().getDescriptorBinding(dst_binding).type;
	write_ds.pBufferInfo = buffer_infos_.back().get();
	
	write_operations_.push_back(write_ds);
}

const std::vector<vk::CopyDescriptorSet>& DescriptorsUpdate::getCopyOperations() const {
	return copy_operations_;
}

const std::vector<vk::WriteDescriptorSet>& DescriptorsUpdate::getWriteOperations() const {
	return write_operations_;
}

}