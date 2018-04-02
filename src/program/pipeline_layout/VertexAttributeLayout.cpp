#include "program/pipeline_layout/VertexAttributeLayout.h"

namespace vkr {

VertexAttributeLayout::VertexAttributeLayout(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset, uint32_t stride, vk::VertexInputRate input_rate)
	: location_(location), binding_(binding), format_(format), offset_(offset), stride_(stride), input_rate_(input_rate) {
}

vk::VertexInputBindingDescription VertexAttributeLayout::getVkBindingDescription() const {
	return vk::VertexInputBindingDescription(binding_, stride_, input_rate_);
}

vk::VertexInputAttributeDescription VertexAttributeLayout::getVkAttributeDescription() const {
	return vk::VertexInputAttributeDescription(location_, binding_, format_, offset_);
}

uint32_t VertexAttributeLayout::getLocation() const {
	return location_;
}

uint32_t VertexAttributeLayout::getBinding() const {
	return binding_;
}

vk::Format VertexAttributeLayout::getFormat() const {
	return format_;
}

uint32_t VertexAttributeLayout::getOffset() const {
	return offset_;
}

uint32_t VertexAttributeLayout::getStride() const {
	return stride_;
}

vk::VertexInputRate VertexAttributeLayout::getInputRate() const {
	return input_rate_;
}

} ///!	namespace vkr