#include "logi/descriptors/DescriptorPool.h"
#include "logi/base/Exceptions.h"

namespace logi {

DescriptorPool::DescriptorPool() : DependentDestroyableHandle({}, false) {}

DescriptorPool::DescriptorPool(std::weak_ptr<HandleManager>& owner, const vk::Device& device, const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags)
	: DependentDestroyableHandle(owner), data_(std::make_shared<DescriptorPoolData>(pool_sizes, flags)), vk_descriptor_pool_(nullptr), handle_manager_(std::make_shared<HandleManager>()) {
	std::vector<vk::DescriptorPoolSize> vk_pool_sizes = pool_sizes.getVkDescriptorPoolSizes();

	// Descriptor pool configuration.
	vk::DescriptorPoolCreateInfo descriptor_pool_ci{};
	descriptor_pool_ci.maxSets = pool_sizes.num_sets;
	descriptor_pool_ci.pPoolSizes = vk_pool_sizes.data();
	descriptor_pool_ci.poolSizeCount = static_cast<uint32_t>(vk_pool_sizes.size());
	descriptor_pool_ci.flags = flags;

	// Create descriptor pool.
	vk_descriptor_pool_ = std::make_shared<ManagedVkDescriptorPool>(device, device.createDescriptorPool(descriptor_pool_ci));
}

void DescriptorPool::freeDescriptorSets(const std::vector<DescriptorSet>& descriptor_sets) const {
    if (data_->flags & vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet) {
		throw IllegalInvocation("Tried to free descriptor sets from the pool that was not created with free descriptor set flag.");
    }

	std::vector<vk::DescriptorSet> vk_descriptor_sets;
	vk_descriptor_sets.reserve(descriptor_sets.size());

    for (const DescriptorSet& set : descriptor_sets) {
		vk_descriptor_sets.push_back(set.getVkHandle());
		handle_manager_->destroyHandle(set);
    }

    const vk::Device& device = vk_descriptor_pool_->getOwner();
	device.freeDescriptorSets(vk_descriptor_pool_->get(), static_cast<uint32_t>(vk_descriptor_sets.size()), vk_descriptor_sets.data());
}

void DescriptorPool::reset(const vk::DescriptorPoolResetFlags& reset_flags) const {
	const vk::Device& device = vk_descriptor_pool_->getOwner();
	handle_manager_->destroyAllHandles();
	device.resetDescriptorPool(vk_descriptor_pool_->get(), reset_flags);
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
