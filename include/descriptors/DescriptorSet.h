/*
 * Descriptor set
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DESCRIPTORS_DESCRIPTOR_SET_H
#define DESCRIPTORS_DESCRIPTOR_SET_H

#include <vulkan/vulkan.hpp>
#include "program/layout/PipelineLayout.h"

namespace logi {


class DescriptorSet : public DependentDestroyableHandle {
public:
	DescriptorSet(const std::weak_ptr<HandleManager>& owner, const vk::DescriptorSet& vk_descriptor_set, const DescriptorSetLayout& descriptor_set_layout);

	const vk::DescriptorSet& getVkHandle() const;

	const DescriptorSetLayout& getLayout() const;

	// TODO: Free DescriptorSet
	void destroy() const override;

private:
	std::shared_ptr<vk::DescriptorSet> vk_descriptor_set_;	///< Descriptor set Vulkan handle.
	std::shared_ptr<DescriptorSetLayout> layout_;			///< Vulkan device layout.
};


}
#endif

