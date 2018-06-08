#include "descriptors/DescriptorSet.h"

namespace logi {

	

DescriptorSet::DescriptorSet(const std::weak_ptr<HandleManager>& owner, const vk::DescriptorSet& vk_descriptor_set, const DescriptorSetLayout& descriptor_set_layout) 
	: DependentDestroyableHandle(owner), vk_descriptor_set_(std::make_shared<vk::DescriptorSet>(vk_descriptor_set)), layout_(std::make_shared<DescriptorSetLayout>(descriptor_set_layout)) {}

const vk::DescriptorSet& DescriptorSet::getVkHandle() const {
	// Check if the handle is still valid.
	if (!alive()) {
		throw std::runtime_error("Calls 'getVkHandle' on destroyed DescriptorSet handle.");
	}

	return *vk_descriptor_set_;
}

const DescriptorSetLayout& DescriptorSet::getLayout() const {
	return *layout_;
}

void DescriptorSet::destroy() const {
	DependentDestroyableHandle::destroy();
}

}
