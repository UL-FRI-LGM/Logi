#include "program/layout/DescriptorCount.h"

namespace logi {

DescriptorsCount::DescriptorsCount(uint32_t num_sets, uint32_t samplers, uint32_t combined_image_samplers, uint32_t sampled_images, uint32_t storage_images, uint32_t uniform_texel_buffers,
	uint32_t storage_texel_buffers, uint32_t uniform_buffers, uint32_t storage_buffers, uint32_t uniform_buffers_dynamic, uint32_t storage_buffers_dynamic, uint32_t input_attachments)
	: num_sets(num_sets), samplers(samplers), combined_image_samplers(combined_image_samplers), sampled_images(sampled_images), storage_images(storage_images),
	uniform_texel_buffers(uniform_texel_buffers), storage_texel_buffers(storage_texel_buffers), uniform_buffers(uniform_buffers), storage_buffers(storage_buffers),
	uniform_buffers_dynamic(uniform_buffers_dynamic), storage_buffers_dynamic(storage_buffers_dynamic), input_attachments(input_attachments) {
}

DescriptorsCount DescriptorsCount::operator+(const DescriptorsCount& other) const {
	return DescriptorsCount(num_sets + other.num_sets, samplers + other.samplers, combined_image_samplers + other.combined_image_samplers, sampled_images + other.sampled_images, storage_images + other.storage_images, uniform_texel_buffers + other.uniform_texel_buffers,
		storage_texel_buffers + other.storage_texel_buffers, uniform_buffers + other.uniform_buffers, storage_buffers + other.storage_buffers, uniform_buffers_dynamic + other.uniform_buffers_dynamic, storage_buffers_dynamic + other.storage_buffers_dynamic, input_attachments + other.input_attachments);
}

DescriptorsCount& DescriptorsCount::operator+=(const DescriptorsCount& other) {
	num_sets += other.num_sets;
	samplers += other.samplers;
	combined_image_samplers += other.combined_image_samplers;
	sampled_images += other.sampled_images;
	storage_images += other.storage_images;
	uniform_texel_buffers += other.uniform_texel_buffers;
	storage_texel_buffers += other.storage_texel_buffers;
	uniform_buffers += other.uniform_buffers;
	storage_buffers += other.storage_buffers;
	uniform_buffers_dynamic += other.uniform_buffers_dynamic;
	storage_buffers_dynamic += other.storage_buffers_dynamic;
	input_attachments += other.input_attachments;

	return *this;
}

DescriptorsCount DescriptorsCount::operator*(uint32_t multipler) const {
	return DescriptorsCount(num_sets * multipler, samplers * multipler, combined_image_samplers * multipler, sampled_images * multipler, storage_images * multipler, uniform_texel_buffers * multipler,
		storage_texel_buffers * multipler, uniform_buffers * multipler, storage_buffers * multipler, uniform_buffers_dynamic * multipler, storage_buffers_dynamic * multipler, input_attachments * multipler);
}

DescriptorsCount DescriptorsCount::operator*=(uint32_t multipler) {
	num_sets *= multipler;
	samplers *= multipler;
	combined_image_samplers *= multipler;
	sampled_images *= multipler;
	storage_images *= multipler;
	uniform_texel_buffers *= multipler;
	storage_texel_buffers *= multipler;
	uniform_buffers *= multipler;
	storage_buffers *= multipler;
	uniform_buffers_dynamic *= multipler;
	storage_buffers_dynamic *= multipler;
	input_attachments *= multipler;

	return *this;
}

std::vector<vk::DescriptorPoolSize> DescriptorsCount::getVkDescriptorPoolSizes() const {
	std::vector<vk::DescriptorPoolSize> vk_pool_sizes{};

	if (samplers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eSampler, samplers));
	}
	if (combined_image_samplers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, combined_image_samplers));
	}
	if (sampled_images > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, sampled_images));
	}
	if (storage_images > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, storage_images));
	}
	if (uniform_texel_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, uniform_texel_buffers));
	}
	if (storage_texel_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, storage_texel_buffers));
	}
	if (uniform_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, uniform_buffers));
	}
	if (storage_buffers > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, storage_buffers));
	}
	if (uniform_buffers_dynamic > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, uniform_buffers_dynamic));
	}
	if (storage_buffers_dynamic > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, storage_buffers_dynamic));
	}
	if (input_attachments > 0) {
		vk_pool_sizes.emplace_back(vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, input_attachments));
	}

	return vk_pool_sizes;
}

} ///!	namespace vkr