#include "program/layout/DescriptorSetLayout.h"

namespace logi {

DescriptorSetLayout::DescriptorSetLayout(const vk::Device& device, const std::vector<internal::DescriptorBindingInitializer>& binding_intializers) 
	: bindings_(std::make_shared<std::vector<DescriptorBindingLayout>>()), descriptors_count_(std::make_shared<DescriptorsCount>(1u)) {

	for (const auto& initializer : binding_intializers) {
		bindings_->emplace_back(DescriptorBindingLayout(initializer));
		incrementDescriptorCount(initializer.type);
	}

	// Generate Vulkan binding layouts.
	std::vector<vk::DescriptorSetLayoutBinding> vk_bindings{};
	vk_bindings.reserve(bindings_->size());

	for (auto it = bindings_->begin(); it != bindings_->end(); ++it) {
		vk_bindings.emplace_back(it->createVkBinding());
	}

	// Create descriptor set layout.
	vk::DescriptorSetLayoutCreateInfo dsl_ci;
	dsl_ci.bindingCount = vk_bindings.size();
	dsl_ci.pBindings = vk_bindings.data();

	vk_descriptor_set_layout_ = std::make_shared<ManagedVkDescriptorSetLayout>(device, device.createDescriptorSetLayout(dsl_ci));
}

void DescriptorSetLayout::incrementDescriptorCount(const vk::DescriptorType& type) const {
	switch (type) {
	case vk::DescriptorType::eSampler:
		descriptors_count_->samplers++;
		break;
	case vk::DescriptorType::eCombinedImageSampler:
		descriptors_count_->combined_image_samplers++;
		break;
	case vk::DescriptorType::eSampledImage:
		descriptors_count_->sampled_images++;
		break;
	case vk::DescriptorType::eStorageImage:
		descriptors_count_->storage_images++;
		break;
	case vk::DescriptorType::eUniformTexelBuffer:
		descriptors_count_->uniform_texel_buffers++;
		break;
	case vk::DescriptorType::eStorageTexelBuffer:
		descriptors_count_->storage_texel_buffers++;
		break;
	case vk::DescriptorType::eUniformBuffer:
		descriptors_count_->uniform_buffers++;
		descriptors_count_->uniform_buffers_dynamic++;
		break;
	case vk::DescriptorType::eStorageBuffer:
		descriptors_count_->storage_buffers++;
		descriptors_count_->storage_buffers_dynamic++;
		break;
	case vk::DescriptorType::eInputAttachment:
		descriptors_count_->input_attachments++;
		break;
	}
}

void DescriptorSetLayout::free() {
	vk_descriptor_set_layout_->destroy();
}

const DescriptorBindingLayout& DescriptorSetLayout::getDescriptorBinding(const uint32_t binding) const {
	for (auto it = bindings_->begin(); it != bindings_->end(); ++it) {
		if (it->binding == binding) {
			return *it;
		}
		if (it->binding > binding) {
			break;
		}
	}

	throw std::runtime_error("Descriptor does not have binding with the given binding index.");
}


const vk::DescriptorSetLayout& DescriptorSetLayout::getVkHandle() const {
	if (!alive()) {
		throw std::runtime_error("Called getVkHandle on a destroyed DescriptorSetLayout.");
	}

	return vk_descriptor_set_layout_->get();
}

const DescriptorsCount& DescriptorSetLayout::getDescriptorsCount() const {
	return *descriptors_count_;
}


} ///!	namespace vkr