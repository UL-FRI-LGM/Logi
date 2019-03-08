#include "logi/program/layout/VertexAttributeLayout.h"

namespace logi {

VertexAttributeLayout::VertexAttributeLayout(const uint32_t location, const uint32_t binding, const vk::Format format, const uint32_t offset, const uint32_t stride, const vk::VertexInputRate input_rate)
	: location(location), binding(binding), format(format), offset(offset), stride(stride), input_rate(input_rate) {
}

vk::VertexInputBindingDescription VertexAttributeLayout::getVkBindingDescription() const {
	return vk::VertexInputBindingDescription(binding, stride, input_rate);
}

vk::VertexInputAttributeDescription VertexAttributeLayout::getVkAttributeDescription() const {
	return vk::VertexInputAttributeDescription(location, binding, format, offset);
}

} ///!	namespace logi