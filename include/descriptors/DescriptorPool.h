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
#include <list>
#include "program/layout/PipelineLayout.h"

namespace vkr {

class DescriptorSet {
public:
	DescriptorSet(const vk::DescriptorSet& descriptor_set, const DescriptorSetLayout* descriptor_set_layout);

	const vk::DescriptorSet& getVkDescriptorSet() const;

	const DescriptorSetLayout* getLayout() const;

private:
	vk::DescriptorSet descriptor_set_;	///< Descriptor set Vulkan handle.
	const DescriptorSetLayout* layout_;	///< Vulkan device layout.
};

class DescriptorPool {
public:
	DescriptorPool(const vk::Device& device, const DescriptorsCount& pool_sizes, bool releasable_sets);

	DescriptorSet* createDescriptorSet(const DescriptorSetLayout* set_layout);

	~DescriptorPool();
private:
	// Device reference
	vk::Device device_;

	// Configuration
	DescriptorsCount pool_sizes_;

	bool releasable_sets_;
	vk::DescriptorPool pool_; ///< Descriptor pool object.
	std::list<std::unique_ptr<DescriptorSet>> allocated_descriptor_sets_;
};

}


#endif
