#include "descriptors/DescriptorSet.h"
#include "base/Exceptions.h"

namespace logi {


DescriptorSet::DescriptorSet() : Handle(false) {
}

DescriptorSet::DescriptorSet(const vk::DescriptorSet& vk_descriptor_set, const DescriptorSetLayout& descriptor_set_layout) 
	: data_(std::make_shared<DescriptorSetData>(vk_descriptor_set, descriptor_set_layout)) {}

const vk::DescriptorSet& DescriptorSet::getVkHandle() const {
	// Check if the handle is still valid.
	if (!alive()) {
		throw IllegalInvocation("Calls 'getVkHandle' on destroyed DescriptorSet handle.");
	}

	return data_->vk_descriptor_set;
}

const DescriptorSetLayout& DescriptorSet::getLayout() const {
	return data_->layout;
}

}
