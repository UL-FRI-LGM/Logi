#include "logi/program/layout/PushConstantRange.h"

namespace logi {
namespace internal {

PushConstantRangeInitializer::PushConstantRangeInitializer(uint32_t offset, uint32_t size, vk::ShaderStageFlags stages) : offset(offset), size(size), stages(stages) {
}

} ///!	namespace internal

PushConstantRange::PushConstantRange(const internal::PushConstantRangeInitializer& initializer) : offset_(initializer.offset), size_(initializer.size), stages_(initializer.stages), vk_handle_(vk::PushConstantRange(stages_, offset_, size_)) {
}

vk::ShaderStageFlags PushConstantRange::getStages() const {
	return stages_;
}

uint32_t PushConstantRange::getOffset() const {
	return offset_;
}

uint32_t PushConstantRange::getSize() const {
	return size_;
}

const vk::PushConstantRange& PushConstantRange::getVkHandle() const {
	return vk_handle_;
}

} ///!	namespace vkr