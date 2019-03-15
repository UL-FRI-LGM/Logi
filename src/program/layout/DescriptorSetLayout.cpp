#include "logi/program/layout/DescriptorSetLayout.h"

namespace logi {

DescriptorSetLayout::DescriptorSetLayout(const PipelineLayout& pipeline_layout, const vk::Device& device,
                                         const std::vector<internal::DescriptorBindingInitializer>& binding_intializers)
  : OwnedHandle<PipelineLayout>(pipeline_layout, true), data_(nullptr) {
  std::vector<DescriptorBindingLayout> bindings;
  bindings.reserve(binding_intializers.size());
  DescriptorsCount descriptors_count(1u);

  for (const auto& initializer : binding_intializers) {
    bindings.emplace_back(DescriptorBindingLayout(initializer));
    descriptors_count.incrementCount(initializer.type);
  }

  // Generate Vulkan binding layouts.
  std::vector<vk::DescriptorSetLayoutBinding> vk_bindings;
  vk_bindings.reserve(bindings.size());

  for (auto& binding : bindings) {
    vk_bindings.emplace_back(binding.createVkBinding());
  }

  // Create descriptor set layout.
  vk::DescriptorSetLayoutCreateInfo dsl_ci;
  dsl_ci.bindingCount = static_cast<uint32_t>(vk_bindings.size());
  dsl_ci.pBindings = vk_bindings.data();

  data_ = std::make_shared<Data>(ManagedVkDescriptorSetLayout(device, device.createDescriptorSetLayout(dsl_ci)),
                                 std::move(bindings), descriptors_count);
}

const DescriptorBindingLayout& DescriptorSetLayout::getDescriptorBinding(const uint32_t binding) const {
  checkForNullHandleInvocation("DescriptorSetLayout", "getDescriptorBinding");
  for (auto& binding_entry : data_->bindings) {
    if (binding_entry.binding == binding) {
      return binding_entry;
    }

    if (binding_entry.binding > binding) {
      break;
    }
  }

  throw std::runtime_error("Descriptor does not have binding with the given binding index.");
}

const vk::DescriptorSetLayout& DescriptorSetLayout::getVkHandle() const {
  checkForNullHandleInvocation("DescriptorSetLayout", "getVkHandle");
  return data_->vk_desc_layout.get();
}

const DescriptorsCount& DescriptorSetLayout::getDescriptorsCount() const {
  checkForNullHandleInvocation("DescriptorSetLayout", "getDescriptorsCount");
  return data_->descriptors_count;
}

void DescriptorSetLayout::free() {
  if (valid()) {
    data_->vk_desc_layout.destroy();
    OwnedHandle<PipelineLayout>::free();
  }
}

DescriptorSetLayout::Data::Data(DescriptorSetLayout::ManagedVkDescriptorSetLayout vk_desc_layout,
                                std::vector<DescriptorBindingLayout> bindings,
                                const DescriptorsCount& descriptors_count)
  : vk_desc_layout(vk_desc_layout), bindings(std::move(bindings)), descriptors_count(descriptors_count) {}

} // namespace logi