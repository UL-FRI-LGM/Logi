#include "logi/descriptors/DescriptorPool.h"
#include "logi/base/Exceptions.h"
#include "logi/base/LogicalDevice.h"

namespace logi {

DescriptorPool::DescriptorPool(const LogicalDevice& device, const DescriptorsCount& pool_sizes,
                               const vk::DescriptorPoolCreateFlags& flags)
  : DestroyableOwnedHandle(device, true), data_(nullptr) {
  std::vector<vk::DescriptorPoolSize> vk_pool_sizes = pool_sizes.getVkDescriptorPoolSizes();

  // Descriptor pool configuration.
  vk::DescriptorPoolCreateInfo descriptor_pool_ci{};
  descriptor_pool_ci.maxSets = pool_sizes.num_sets;
  descriptor_pool_ci.pPoolSizes = vk_pool_sizes.data();
  descriptor_pool_ci.poolSizeCount = static_cast<uint32_t>(vk_pool_sizes.size());
  descriptor_pool_ci.flags = flags;

  // Create descriptor pool.
  vk::Device vk_device = device;
  data_ = std::make_shared<Data>(ManagedVkDescriptorPool(vk_device, vk_device.createDescriptorPool(descriptor_pool_ci)),
                                 pool_sizes, flags);
}

void DescriptorPool::freeDescriptorSets(const std::vector<DescriptorSet>& descriptor_sets) const {
  checkForNullHandleInvocation("DescriptorPool", "freeDescriptorSets");

  if (data_->flags & vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet) {
    throw IllegalInvocation("Tried to free descriptor sets from the pool that was not created with free descriptor set "
                            "flag.");
  }

  std::vector<vk::DescriptorSet> vk_descriptor_sets;
  vk_descriptor_sets.reserve(descriptor_sets.size());

  for (const DescriptorSet& set : descriptor_sets) {
    vk_descriptor_sets.push_back(set.getVkHandle());
    HandleGenerator<DescriptorPool, DescriptorSet>::destroyHandle(set);
  }

  vk::Device device = getOwner<LogicalDevice>();
  device.freeDescriptorSets(device, static_cast<uint32_t>(vk_descriptor_sets.size()), vk_descriptor_sets.data());
}

void DescriptorPool::reset(const vk::DescriptorPoolResetFlags& reset_flags) const {
  checkForNullHandleInvocation("DescriptorPool", "reset");

  vk::Device vk_device = getOwner<LogicalDevice>();
  HandleGenerator<DescriptorPool, DescriptorSet>::destroyAllHandles();
  vk_device.resetDescriptorPool(data_->vk_descriptor_pool.get(), reset_flags);
}

void DescriptorPool::free() {
  if (valid()) {
    // Destroy dependent handles and descriptor pool.
    HandleGenerator<DescriptorPool, DescriptorSet>::destroyAllHandles();
    data_->vk_descriptor_pool.destroy();

    DestroyableOwnedHandle<LogicalDevice>::free();
  }
}

DescriptorPool::Data::Data(ManagedVkDescriptorPool vk_descriptor_pool, const DescriptorsCount& pool_sizes,
                           const vk::DescriptorPoolCreateFlags& flags)
  : vk_descriptor_pool(vk_descriptor_pool), pool_sizes(pool_sizes), flags(flags) {}

} // namespace logi
