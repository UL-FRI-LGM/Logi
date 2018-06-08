#include "descriptors/DescriptorPool.h"

namespace logi {

DescriptorPool::DescriptorPool() : DependentDestroyableHandle({}, false) {}

DescriptorPool::DescriptorPool(std::weak_ptr<HandleManager>& owner, const vk::Device& device, const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags)
	: DependentDestroyableHandle(owner), data_(std::make_shared<DescriptorPoolData>(pool_sizes, flags)), vk_descriptor_pool_(nullptr), handle_manager_(std::make_shared<HandleManager>()) {
	std::vector<vk::DescriptorPoolSize> vk_pool_sizes = pool_sizes.getVkDescriptorPoolSizes();

	// Descriptor pool configuration.
	vk::DescriptorPoolCreateInfo descriptor_pool_ci{};
	descriptor_pool_ci.maxSets = pool_sizes.num_sets;
	descriptor_pool_ci.pPoolSizes = vk_pool_sizes.data();
	descriptor_pool_ci.poolSizeCount = vk_pool_sizes.size();
	descriptor_pool_ci.flags = flags;

	// Create descriptor pool.
	vk_descriptor_pool_ = std::make_shared<ManagedVkDescriptorPool>(device, device.createDescriptorPool(descriptor_pool_ci));
}

DescriptorSet DescriptorPool::createDescriptorSet(const DescriptorSetLayout& set_layout) const {
	// Allocate descriptor set.
	vk::DescriptorSetAllocateInfo set_alloc_info{};
	set_alloc_info.descriptorPool = vk_descriptor_pool_->get();
	set_alloc_info.descriptorSetCount = 1;
	set_alloc_info.pSetLayouts = &set_layout.getVkHandle();

	return handle_manager_->createHandle<DescriptorSet>(vk_descriptor_pool_->getOwner().allocateDescriptorSets(set_alloc_info)[0], set_layout);
}

void DescriptorPool::free() {
	// Destroy dependent handles and descriptor pool.
	handle_manager_->destroyAllHandles();
	vk_descriptor_pool_->destroy();

	DependentDestroyableHandle::free();
}

DescriptorPool::DescriptorPoolData::DescriptorPoolData(const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags)
	: pool_sizes(pool_sizes), flags(flags) {}

}
