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

#include "program/layout/PipelineLayout.h"
#include "base/ManagedResource.h"
#include "descriptors/DescriptorSet.h"
#include "base/Handle.h"


namespace logi {



class DescriptorPool : public DependentDestroyableHandle {
public:
	DescriptorPool();

	DescriptorPool(std::weak_ptr<HandleManager>& owner, const vk::Device& device, const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags);

	DescriptorSet createDescriptorSet(const DescriptorSetLayout& set_layout) const;

protected:
	void free() override;

private:
	using ManagedVkDescriptorPool = ManagedResource<vk::Device, vk::DescriptorPool, vk::DispatchLoaderStatic, &vk::Device::destroyDescriptorPool>;

	/**
	* @brief	Contains configuration for DescriptorPool.
	*/
	struct DescriptorPoolData {
		/**
		 * @brief	Default constructor. Initializes values to defaults.
		 *
		 * @param	pool_sizes	Specifies required pool size for each descriptor type.
		 * @param	flags		A bitmask of vk::DescriptorPoolCreateFlagBits describing additional parameters of the DescriptorPool.
		 */
		DescriptorPoolData(const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags);

		DescriptorsCount pool_sizes;			///< Specifies required pool size for each descriptor type.
		vk::DescriptorPoolCreateFlags flags;	///< A bitmask of vk::DescriptorPoolCreateFlagBits describing additional parameters of the DescriptorPool.
	};

	std::shared_ptr<DescriptorPoolData> data_;						///< Contains descriptor pool data.
	std::shared_ptr<ManagedVkDescriptorPool> vk_descriptor_pool_;	///< Vulkan DescriptorPool handle.
	std::shared_ptr<HandleManager> handle_manager_;			///< Manages DescriptorSet handles.
};

}


#endif
