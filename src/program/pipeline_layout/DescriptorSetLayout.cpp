#include "program/pipeline_layout/DescriptorSetLayout.h"

namespace vkr {

DescriptorSetLayout::DescriptorSetLayout(const vk::Device& device, const std::vector<internal::DescriptorBindingInitializer>& binding_intializers) : device_(device), vk_handle_(nullptr), bindings_(), descriptors_count_(1u) {
	for (const internal::DescriptorBindingInitializer& initializer : binding_intializers) {
		bindings_.emplace_back(std::make_unique<DescriptorBindingLayout>(initializer));
		incrementDescriptorCount(initializer.type);
	}

	// Generate Vulkan binding layouts.
	std::vector<vk::DescriptorSetLayoutBinding> vk_bindings{};
	vk_bindings.reserve(bindings_.size());

	for (auto it = bindings_.begin(); it != bindings_.end(); it++) {
		vk_bindings.emplace_back((*it)->getVkHandle());
	}

	// Create descriptor set layout.
	vk::DescriptorSetLayoutCreateInfo dsl_ci;
	dsl_ci.bindingCount = vk_bindings.size();
	dsl_ci.pBindings = vk_bindings.data();

	vk_handle_ = device_.createDescriptorSetLayout(dsl_ci);
}

void DescriptorSetLayout::incrementDescriptorCount(const vk::DescriptorType& type) {
	switch (type) {
	case vk::DescriptorType::eSampler:
		descriptors_count_.samplers++;
		break;
	case vk::DescriptorType::eCombinedImageSampler:
		descriptors_count_.combined_image_samplers++;
		break;
	case vk::DescriptorType::eSampledImage:
		descriptors_count_.sampled_images++;
		break;
	case vk::DescriptorType::eStorageImage:
		descriptors_count_.storage_images++;
		break;
	case vk::DescriptorType::eUniformTexelBuffer:
		descriptors_count_.uniform_texel_buffers++;
		break;
	case vk::DescriptorType::eStorageTexelBuffer:
		descriptors_count_.storage_texel_buffers++;
		break;
	case vk::DescriptorType::eUniformBuffer:
		descriptors_count_.uniform_buffers++;
		descriptors_count_.uniform_buffers_dynamic++;
		break;
	case vk::DescriptorType::eStorageBuffer:
		descriptors_count_.storage_buffers++;
		descriptors_count_.storage_buffers_dynamic++;
		break;
	case vk::DescriptorType::eInputAttachment:
		descriptors_count_.input_attachments++;
		break;
	}
}

const DescriptorBindingLayout* DescriptorSetLayout::getDescriptorBinding(uint32_t binding) const {
	for (auto it = bindings_.begin(); it != bindings_.end(); it++) {
		if ((*it)->getBinding() == binding) {
			return (*it).get();
		}
		else if ((*it)->getBinding() > binding) {
			break;
		}
	}

	return nullptr;
}


const vk::DescriptorSetLayout& DescriptorSetLayout::getVkHandle() const {
	return vk_handle_;
}

const DescriptorsCount& DescriptorSetLayout::getDescriptorsCount() const {
	return descriptors_count_;
}

DescriptorSetLayout::~DescriptorSetLayout() {
	if (vk_handle_) {
		device_.destroyDescriptorSetLayout(vk_handle_);
	}
}

} ///!	namespace vkr