#include "descriptors/DescriptorPool.h"

namespace vkr {

DescriptorSet::DescriptorSet(const vk::DescriptorSet& descriptor_set, const DescriptorSetLayout* descriptor_set_layout)
	: descriptor_set_(descriptor_set), layout_(descriptor_set_layout) {
}

const vk::DescriptorSet& DescriptorSet::getVkDescriptorSet() const {
	return descriptor_set_;
}

const DescriptorSetLayout* DescriptorSet::getLayout() const {
	return layout_;
}

DescriptorPool::DescriptorPool(const vk::Device& device, const DescriptorsCount& pool_sizes, bool releasable_sets) : device_(device), pool_sizes_(pool_sizes), releasable_sets_(false), pool_(nullptr), allocated_descriptor_sets_() {
	std::vector<vk::DescriptorPoolSize> vk_pool_sizes = pool_sizes.getVkDescriptorPoolSizes();

	// Descriptor pool configuration.
	vk::DescriptorPoolCreateInfo pool_info{};
	pool_info.maxSets = pool_sizes.num_sets;
	pool_info.pPoolSizes = vk_pool_sizes.data();
	pool_info.poolSizeCount = vk_pool_sizes.size();

	if (releasable_sets) {
		pool_info.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
	}

	// Create descriptor pool.
	pool_ = device_.createDescriptorPool(pool_info);
}

DescriptorSet* DescriptorPool::createDescriptorSet(const DescriptorSetLayout* set_layout) {
	// Allocate descriptor set.
	vk::DescriptorSetAllocateInfo set_alloc_info{};
	set_alloc_info.descriptorPool = pool_;
	set_alloc_info.descriptorSetCount = 1;
	set_alloc_info.pSetLayouts = &set_layout->getVkHandle();

	allocated_descriptor_sets_.emplace_back(std::make_unique<DescriptorSet>(device_.allocateDescriptorSets(set_alloc_info)[0], set_layout));

	return allocated_descriptor_sets_.back().get();
}

DescriptorPool::~DescriptorPool() {
	device_.destroyDescriptorPool(pool_);
}

}