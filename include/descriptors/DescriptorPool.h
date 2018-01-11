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
#include "program_management/PipelineLayout.h"

namespace vkr {

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