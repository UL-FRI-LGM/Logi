#include "program/pipeline_layout/DescriptorBindingLayout.h"

namespace vkr {
namespace internal {

DescriptorBindingInitializer::DescriptorBindingInitializer(uint32_t binding, vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stages) : binding(binding), type(type), count(count), stages(stages) {
}

} ///!	namespace internal

DescriptorBindingLayout::DescriptorBindingLayout(const internal::DescriptorBindingInitializer& binding_initializer) 
	: binding_(binding_initializer.binding), type_(binding_initializer.type), count_(binding_initializer.count), stages_(binding_initializer.stages), vk_handle_(vk::DescriptorSetLayoutBinding(binding_, type_, count_, stages_, nullptr)) {
}

uint32_t DescriptorBindingLayout::getBinding() const {
	return binding_;
}

vk::DescriptorType DescriptorBindingLayout::getDescriptorType() const {
	return type_;
}

uint32_t DescriptorBindingLayout::getCount() const {
	return count_;
}

const vk::ShaderStageFlags& DescriptorBindingLayout::getShaderStages() const {
	return stages_;
}

const vk::DescriptorSetLayoutBinding& DescriptorBindingLayout::getVkHandle() const {
	return vk_handle_;
}

} ///!	namespace vkr