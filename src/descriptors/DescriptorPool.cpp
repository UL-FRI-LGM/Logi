#include "descriptors/DescriptorPool.h"

namespace vkr {

DescriptorPool::DescriptorPool(vk::Device device, const DescriptorsCount& pool_sizes, bool releasable_sets) : device_(device), pool_sizes_(pool_sizes), pool_(nullptr), cached_ds_() {
	std::vector<vk::DescriptorPoolSize> vk_pool_sizes = pool_sizes.getVkDescriptorPoolSizes();

	// Descriptor pool configuration.
	vk::DescriptorPoolCreateInfo pool_info{};
	pool_info.maxSets = pool_sizes.num_sets;
	pool_info.pPoolSizes = vk_pool_sizes.data();
	pool_info.poolSizeCount = vk_pool_sizes.size();

	// Create descriptor pool.
	pool_ = device_.createDescriptorPool(pool_info);
}

vk::DescriptorSet DescriptorPool::getDescriptorSet(const vk::DescriptorSetLayout& set_layout) {
	// The set was already allocated. Return its handle.
	auto it = cached_ds_.find(static_cast<VkDescriptorSetLayout>(set_layout));
	if (it != cached_ds_.end()) {
		return it->second;
	}

	// Allocate descriptor set.
	vk::DescriptorSetAllocateInfo set_alloc_info{};
	set_alloc_info.descriptorPool = pool_;
	set_alloc_info.descriptorSetCount = 1;
	set_alloc_info.pSetLayouts = &set_layout;

	vk::DescriptorSet set = device_.allocateDescriptorSets(set_alloc_info)[0];
	cached_ds_[static_cast<VkDescriptorSetLayout>(set_layout)] = set;
	return set;
}

DescriptorPool::~DescriptorPool() {
	device_.destroyDescriptorPool(pool_);
}

}