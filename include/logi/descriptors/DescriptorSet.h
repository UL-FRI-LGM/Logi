/*
 * Descriptor set
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DESCRIPTORS_DESCRIPTOR_SET_H
#define DESCRIPTORS_DESCRIPTOR_SET_H

#include <utility>
#include <vulkan/vulkan.hpp>
#include "logi/program/layout/PipelineLayout.h"

namespace logi {


/**
 * @brief	Handle wrapping the Vulkan descriptor set.
 */
class DescriptorSet : public Handle {
public:
    /**
     * @brief   Default placeholder consturctor.
     */
    DescriptorSet();

    /**
	 * @brief   Store the descriptor set and layout.	
	 *
	 * @param	vk_descriptor_set       Vulkan descriptor set.
	 * @param	descriptor_set_layout   Vulkan descriptor set layout.
	 */
    DescriptorSet(const vk::DescriptorSet& vk_descriptor_set, const DescriptorSetLayout& descriptor_set_layout);

    /**
	 * @brief   Retrieve Vulkan DescriptorSet handle.	
	 *
	 * @return	Vulkan DescriptorSet handle.	
	 */
    const vk::DescriptorSet& getVkHandle() const;

    /**
	 * @brief	Retrieve layout of this DescriptorSet.
	 *
	 * @return	DescriptorSet layout.
	 */
    const DescriptorSetLayout& getLayout() const;

private:
    /**
     * @brief   Structure containing DescriptorSet data.
     */
    struct DescriptorSetData {
        /**
         * @brief	Populates DescriptorSetData.
         *
         * @param	vk_descriptor_set   Vulkan DescriptorSet handle.
         * @param	layout              Descriptor set layout.
         */
        DescriptorSetData(const vk::DescriptorSet& vk_descriptor_set, DescriptorSetLayout layout)
            : vk_descriptor_set(vk_descriptor_set), layout(std::move(layout)) {}

        /**
         * Vulkan DescriptorSet handle.
         */
        vk::DescriptorSet vk_descriptor_set;

        /**
         * Descriptor set layout.
         */
        DescriptorSetLayout layout;
    };

    /**
     * Descriptor set data shared among handle instances.
     */
    std::shared_ptr<DescriptorSetData> data_;
};


}
#endif
