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
#include "base/ExtensionObject.h"

namespace logi {

class DescriptorPool : public DependentDestroyableHandle {
public:

    /**
	 * @brief	Construct placeholder DescriptorPool handle.
	 */
	DescriptorPool();

    /**
	 * @brief   Create and configure Vulkan descriptor pool.	
	 *
	 * @param	owner       Pointer to the handle manager responsible for this descriptor pool.
	 * @param	device      Vulkan device handle.
	 * @param	pool_sizes  Descriptor pool sizes.
	 * @param	flags       Additional DescriptorPool configuration flags.
	 */
	DescriptorPool(std::weak_ptr<HandleManager>& owner, const vk::Device& device, const DescriptorsCount& pool_sizes, const vk::DescriptorPoolCreateFlags& flags);

    /**
     * @brief	Allocate descriptors sets using the given descriptor set layouts.
     *
     * @tparam	ExtensionTypes  Types of extension objects.
     * @param	set_layouts     Descriptor set layout handles.
     * @param	extensions      Variadic allocation info extensions
     * @return	Vector of allocated descriptor sets ordered in the same order as the given layouts.
     */
    template <typename ...ExtensionTypes>
	std::vector<DescriptorSet> allocateDescriptorSets(const std::vector<DescriptorSetLayout>& set_layouts,
	                                                  ExtensionTypes... extensions) const;
	
    /**
     * @brief	Allocate descriptor sets using the given descriptor set layout.
     *
     * @tparam	ExtensionTypes  Types of extension objects.
     * @param	set_layout     Descriptor set layout handle.
     * @param	extensions      Variadic allocation info extensions
     * @return	Allocated descriptor set.
     */
	template <typename ...ExtensionTypes>
	DescriptorSet allocateDescriptorSet(const DescriptorSetLayout& set_layout, ExtensionTypes... extensions) const;

    /**
	 * @brief	Free the given descriptor sets.
	 *
	 * @param	descriptor_sets Vectir of descriptor sets that will be freed.
	 */
	void freeDescriptorSets(const std::vector<DescriptorSet>& descriptor_sets) const;

    /**
	 * @brief	Resets descriptor pool. Frees all descriptor sets allocated from this pool.
	 *
	 * @param	reset_flags Additional reset options.
	 */
	void reset(const vk::DescriptorPoolResetFlags& reset_flags) const;

protected:
    /**
     * @brief   Destroys all dependent handle and frees the pool.
     */
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

        /**
         * Specifies required pool size for each descriptor type.
         */
		DescriptorsCount pool_sizes;

	    /**
	     * A bitmask of vk::DescriptorPoolCreateFlagBits describing additional parameters of the DescriptorPool.
	     */
		vk::DescriptorPoolCreateFlags flags;
	};

    /**
     * Contains descriptor pool data.
     */
	std::shared_ptr<DescriptorPoolData> data_;

    /**
     * Vulkan DescriptorPool handle.
     */
	std::shared_ptr<ManagedVkDescriptorPool> vk_descriptor_pool_;

    /**
     * Manages DescriptorSet handles.
     */
	std::shared_ptr<HandleManager> handle_manager_;
};

template <typename ... ExtensionTypes>
std::vector<DescriptorSet> DescriptorPool::allocateDescriptorSets(const std::vector<DescriptorSetLayout>& set_layouts,
    ExtensionTypes... extensions) const {

    // Generate descriptor set layouts.
	std::vector<vk::DescriptorSetLayout> vk_layouts;
	vk_layouts.reserve(set_layouts.size());

    for (const DescriptorSetLayout& layout : set_layouts) {
		vk_layouts.push_back(layout.getVkHandle());
    }
    
	// Generate allocate info.
	vk::DescriptorSetAllocateInfo set_alloc_info{};
	set_alloc_info.descriptorPool = vk_descriptor_pool_->get();
	set_alloc_info.descriptorSetCount = vk_layouts.size();
	set_alloc_info.pSetLayouts = vk_layouts.data();
	set_alloc_info.pNext = ExtensionObject::buildExtensions(extensions...);

    // Allocate descriptor sets.
	const vk::Device& device = vk_descriptor_pool_->getOwner();
	std::vector<vk::DescriptorSet> vk_descriptor_sets = device.allocateDescriptorSets(set_alloc_info);
	std::vector<DescriptorSet> descriptor_sets;
	descriptor_sets.reserve(vk_descriptor_sets.size());

    // Create handles.
    for (size_t i = 0; i < vk_descriptor_sets.size(); i++) {
		descriptor_sets.emplace_back(handle_manager_->createHandle<DescriptorSet>(vk_descriptor_sets[i], set_layouts[i]));
    }

	return descriptor_sets;
}

template <typename ... ExtensionTypes>
DescriptorSet DescriptorPool::allocateDescriptorSet(const DescriptorSetLayout& set_layout,
    ExtensionTypes... extensions) const {
   
	// Generate allocate info.
	vk::DescriptorSetAllocateInfo set_alloc_info{};
	set_alloc_info.descriptorPool = vk_descriptor_pool_->get();
	set_alloc_info.descriptorSetCount = 1u;
	set_alloc_info.pSetLayouts = &set_layout.getVkHandle();
	set_alloc_info.pNext = ExtensionObject::buildExtensions(extensions...);

	// Allocate descriptor sets.
	const vk::Device& device = vk_descriptor_pool_->getOwner();

	return handle_manager_->createHandle<DescriptorSet>(device.allocateDescriptorSets(set_alloc_info)[0], set_layout);
}

}


#endif
