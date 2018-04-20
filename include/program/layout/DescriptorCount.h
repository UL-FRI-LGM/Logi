#ifndef PROGRAM_LAYOUT_DESCRIPTOR_COUNT_H
#define PROGRAM_LAYOUT_DESCRIPTOR_COUNT_H

#include <vulkan/vulkan.hpp>
#include <vector>

namespace vkr {

/**
* @brief	Holds the number of descriptor sets and the numbers of each descriptor types.
*/
struct DescriptorsCount {
	uint32_t num_sets;					///< Number of descriptor sets.
	uint32_t samplers;					///< Number of descriptors of type Sampler (VK_DESCRIPTOR_TYPE_SAMPLER).
	uint32_t combined_image_samplers;	///< Number of descriptors of type Combined Image Sampler (VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER).
	uint32_t sampled_images;			///< Number of descriptors of type Sampled Image (VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE).
	uint32_t storage_images;			///< Number of descriptors of type Storage Image (VK_DESCRIPTOR_TYPE_STORAGE_IMAGE).
	uint32_t uniform_texel_buffers;		///< Number of descriptors of type Uniform Texel Buffer (VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER).
	uint32_t storage_texel_buffers;		///< Number of descriptors of type Storage Texel Buffer (VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER).
	uint32_t uniform_buffers;			///< Number of descriptors of type Uniform Buffer (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
	uint32_t storage_buffers;			///< Number of descriptors of type Storage Buffer (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER).
	uint32_t uniform_buffers_dynamic;	///< Number of descriptors of type Uniform Buffer Dynamic (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC).
	uint32_t storage_buffers_dynamic;	///< Number of descriptors of type Storage Buffer Dynamic (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC).
	uint32_t input_attachments;			///< Number of descriptors of type Input Attachment (VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT).

	/**
	 * @brief	Default DescriptorCount constructors. Accepts the number of descriptors sets and the numbers of individual descriptor type.
	 *
	 * @param	num_sets			   	(Optional) Number of descriptor sets.
	 * @param	samplers			   	(Optional) Number of descriptors of type Sampler (VK_DESCRIPTOR_TYPE_SAMPLER).
	 * @param	combined_image_samplers	(Optional) Number of descriptors of type Combined Image Sampler (VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER).
	 * @param	sampled_images		   	(Optional) Number of descriptors of type Sampled Image (VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE).
	 * @param	storage_images		   	(Optional) Number of descriptors of type Storage Image (VK_DESCRIPTOR_TYPE_STORAGE_IMAGE).
	 * @param	uniform_texel_buffers  	(Optional) Number of descriptors of type Uniform Texel Buffer (VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER).
	 * @param	storage_texel_buffers  	(Optional) Number of descriptors of type Storage Texel Buffer (VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER).
	 * @param	uniform_buffers		   	(Optional) Number of descriptors of type Uniform Buffer (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
	 * @param	storage_buffers		   	(Optional) Number of descriptors of type Storage Buffer (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
	 * @param	uniform_buffers_dynamic	(Optional) Number of descriptors of type Uniform Buffer Dynamic (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC).
	 * @param	storage_buffers_dynamic	(Optional) Number of descriptors of type Storage Buffer Dynamic (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC).
	 * @param	input_attachments	   	(Optional) Number of descriptors of type Input Attachment (VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT).
	 */
	DescriptorsCount(uint32_t num_sets = 0u, uint32_t samplers = 0u, uint32_t combined_image_samplers = 0u, uint32_t sampled_images = 0u, uint32_t storage_images = 0u, uint32_t uniform_texel_buffers = 0u, uint32_t storage_texel_buffers = 0u,
		uint32_t uniform_buffers = 0u, uint32_t storage_buffers = 0u, uint32_t uniform_buffers_dynamic = 0u, uint32_t storage_buffers_dynamic = 0u, uint32_t input_attachments = 0u);

	/**
	 * @brief	Adds the number of descriptor sets and the numbers of individual descriptor types.
	 *
	 * @param	other Right hand (DescriptorCount) operand.
	 * @return	DescriptorsCount containing the sum.
	 */
	DescriptorsCount operator+(const DescriptorsCount& other) const;

	/**
	 * @brief	Adds the number of descriptor sets and the numbers of individual descriptor types.
	 *
	 * @param	other Right hand (DescriptorCount) operand.
	 * @return	DescriptorsCount containing the sum.
	 */
	DescriptorsCount& operator+=(const DescriptorsCount& other);

	/**
	 * @brief	Multiplies the number of descriptor sets and the numbers of individual descriptor types with the multiplier.
	 *
	 * @param	multiplier Multiplier.
	 * @return	Multiplied DescriptorsCount.
	 */
	DescriptorsCount operator*(uint32_t multipler) const;

	/**
	 * @brief	Multiplies the number of descriptor sets and the numbers of individual descriptor types with the multiplier.
	 *
	 * @param	multiplier Multiplier.
	 * @return	Multiplied DescriptorsCount.
	 */
	DescriptorsCount operator*=(uint32_t multipler);

	/**
	 * @brief	Generates Vulkan DescriptorPoolSize structure and populates it with descriptor counts.
	 *
	 * @return	Vector of DescriptorPoolSize structures.
	 */
	std::vector<vk::DescriptorPoolSize> getVkDescriptorPoolSizes() const;
};

}	///! namespace vkr

#endif ///!	HEADER_GUARD