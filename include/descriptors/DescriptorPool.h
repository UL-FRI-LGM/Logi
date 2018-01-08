/*
* Descriptor pool
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef DESCRIPTORS_DESCRIPTOR_POOL_H
#define DESCRIPTORS_DESCRIPTOR_POOL_H

#include <vulkan/vulkan.hpp>
#include "base/Types.h"
#include <unordered_map>

namespace vkr {

struct DescriptorsCount {
	uint32_t num_sets;

	uint32_t samplers;
	uint32_t combined_image_samplers;
	uint32_t sampled_images;
	uint32_t storage_images;
	uint32_t uniform_texel_buffers;
	uint32_t storage_texel_buffers;
	uint32_t uniform_buffers;
	uint32_t storage_buffers;
	uint32_t uniform_buffers_dynamic;
	uint32_t storage_buffers_dynamic;
	uint32_t input_attachments;

	/**
	 * @brief Default constructor.
	 */
	DescriptorsCount(uint32_t num_sets = 0u, uint32_t samplers = 0u, uint32_t combined_image_samplers = 0u, uint32_t sampled_images = 0u, uint32_t storage_images = 0u, uint32_t uniform_texel_buffers = 0u, uint32_t storage_texel_buffers = 0u,
		uint32_t uniform_buffers = 0u, uint32_t storage_buffers = 0u, uint32_t uniform_buffers_dynamic = 0u, uint32_t storage_buffers_dynamic = 0u, uint32_t input_attachments = 0u);
	
	/**
	 * @brief Adds descriptor and sets sizes of two DescriptorsCounts.
	 *
	 * @return DescriptorsCount containing the sum of sizes.
	 */
	DescriptorsCount operator+(const DescriptorsCount& other) const;

	/**
	 * @brief Adds descriptor and sets sizes of two DescriptorsCounts.
	 *
	 * @return DescriptorsCount containing the sum of sizes.
	 */
	DescriptorsCount& operator+=(const DescriptorsCount& other);

	/**
	 * @brief Multiplies descriptor counter with the given multiplier.
	 *
	 * @return Multiplied DescriptorsCount.
	 */
	DescriptorsCount operator*(uint32_t multipler) const;

	/**
	* @brief Multiplies descriptor counter with the given multiplier.
	*
	* @return Multiplied DescriptorsCount.
	*/
	DescriptorsCount operator*=(uint32_t multipler);

	/**
	 * @brief Generates vector containing Vulkan DescriptorsCounts.
	 *
	 * @return Vector of Vulkan DescriptorsCounts.
	 */
	std::vector<vk::DescriptorPoolSize> getVkDescriptorPoolSizes() const;
};

class DescriptorPool {
public:
	DescriptorPool(vk::Device device, const DescriptorsCount& pool_sizes, bool releasable_sets);

	vk::DescriptorSet getDescriptorSet(const vk::DescriptorSetLayout& set_layout);

	~DescriptorPool();
private:
	// Device reference
	vk::Device device_;

	// Configuration
	DescriptorsCount pool_sizes_;

	vk::DescriptorPool pool_; ///< Descriptor pool object.
	std::unordered_map<VkDescriptorSetLayout, vk::DescriptorSet> cached_ds_;
};

}


#endif