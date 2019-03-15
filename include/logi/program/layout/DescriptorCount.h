#ifndef PROGRAM_LAYOUT_DESCRIPTOR_COUNT_H
#define PROGRAM_LAYOUT_DESCRIPTOR_COUNT_H

#include <vector>
#include <vulkan/vulkan.hpp>

namespace logi {

/**
 * @brief	Holds the number of descriptor sets and the numbers of each descriptor types.
 */
struct DescriptorsCount {
  /**
   * @brief	Default DescriptorCount constructors. Accepts the number of descriptors sets and the numbers of individual
   * descriptor type.
   *
   * @param	num_sets			   	      Number of descriptor sets.
   * @param	samplers			   	      Number of descriptors of type Sampler.
   * @param	combined_image_samplers	Number of descriptors of type Combined Image Sampler.
   * @param	sampled_images		   	  Number of descriptors of type Sampled Image.
   * @param	storage_images		   	  Number of descriptors of type Storage Image.
   * @param	uniform_texel_buffers  	Number of descriptors of type Uniform Texel Buffer.
   * @param	storage_texel_buffers  	Number of descriptors of type Storage Texel Buffer.
   * @param	uniform_buffers		   	  Number of descriptors of type Uniform Buffer.
   * @param	storage_buffers		   	  Number of descriptors of type Storage Buffer.
   * @param	uniform_buffers_dynamic Number of descriptors of type Uniform Buffer Dynamic.
   * @param	storage_buffers_dynamic	Number of descriptors of type Storage Buffer Dynamic.
   * @param	input_attachments	   	  Number of descriptors of type Input Attachment.
   */
  explicit DescriptorsCount(uint32_t num_sets = 0u, uint32_t samplers = 0u, uint32_t combined_image_samplers = 0u,
                            uint32_t sampled_images = 0u, uint32_t storage_images = 0u,
                            uint32_t uniform_texel_buffers = 0u, uint32_t storage_texel_buffers = 0u,
                            uint32_t uniform_buffers = 0u, uint32_t storage_buffers = 0u,
                            uint32_t uniform_buffers_dynamic = 0u, uint32_t storage_buffers_dynamic = 0u,
                            uint32_t input_attachments = 0u);

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
  DescriptorsCount operator*(uint32_t multiplier) const;

  /**
   * @brief	Multiplies the number of descriptor sets and the numbers of individual descriptor types with the multiplier.
   *
   * @param	multiplier Multiplier.
   * @return	Multiplied DescriptorsCount.
   */
  DescriptorsCount operator*=(uint32_t multiplier);

  /**
   * @brief	Generates Vulkan DescriptorPoolSize structure and populates it with descriptor counts.
   *
   * @return	Vector of DescriptorPoolSize structures.
   */
  std::vector<vk::DescriptorPoolSize> getVkDescriptorPoolSizes() const;

  /**
   * @brief	Increment descriptor count of a given type.
   *
   * @param	type	Descriptor type.
   */
  void incrementCount(const vk::DescriptorType& type);

  /**
   * Number of descriptor sets.
   */
  uint32_t num_sets;

  /**
   * Number of descriptors of type Sampler (VK_DESCRIPTOR_TYPE_SAMPLER).
   */
  uint32_t samplers;

  /**
   * Number of descriptors of type Combined Image Sampler (VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER).
   */
  uint32_t combined_image_samplers;

  /**
   * Number of descriptors of type Sampled Image (VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE).
   */
  uint32_t sampled_images;

  /**
   * Number of descriptors of type Storage Image (VK_DESCRIPTOR_TYPE_STORAGE_IMAGE).
   */
  uint32_t storage_images;

  /**
   * Number of descriptors of type Uniform Texel Buffer (VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER).
   */
  uint32_t uniform_texel_buffers;

  /**
   * Number of descriptors of type Storage Texel Buffer (VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER).
   */
  uint32_t storage_texel_buffers;

  /**
   * Number of descriptors of type Uniform Buffer (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
   */
  uint32_t uniform_buffers;

  /**
   * Number of descriptors of type Storage Buffer (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER).
   */
  uint32_t storage_buffers;

  /**
   * Number of descriptors of type Uniform Buffer Dynamic (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC).
   */
  uint32_t uniform_buffers_dynamic;

  /**
   * Number of descriptors of type Storage Buffer Dynamic (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC).
   */
  uint32_t storage_buffers_dynamic;

  /**
   * Number of descriptors of type Input Attachment (VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT).
   */
  uint32_t input_attachments;
};

} // namespace logi

#endif ///!	HEADER_GUARD