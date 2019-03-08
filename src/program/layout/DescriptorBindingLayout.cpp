#include "logi/program/layout/DescriptorBindingLayout.h"

namespace logi {
namespace internal {

DescriptorBindingInitializer::DescriptorBindingInitializer(uint32_t binding, vk::DescriptorType type, uint32_t count, vk::ShaderStageFlags stages) : binding(binding), type(type), count(count), stages(stages) {
}

} ///!	namespace internal

DescriptorBindingLayout::DescriptorBindingLayout(const internal::DescriptorBindingInitializer& binding_initializer) 
	: binding(binding_initializer.binding), type(binding_initializer.type), count(binding_initializer.count), stages(binding_initializer.stages) {
}


vk::DescriptorSetLayoutBinding DescriptorBindingLayout::createVkBinding() const {
	return vk::DescriptorSetLayoutBinding(binding, type, count, stages, nullptr);
}

} ///!	namespace logi