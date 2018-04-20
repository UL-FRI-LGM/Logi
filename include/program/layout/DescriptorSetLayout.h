#ifndef PROGRAM_LAYOUT_DESCRIPTOR_SET_LAYOUT_H
#define PROGRAM_LAYOUT_DESCRIPTOR_SET_LAYOUT_H

#include <vulkan/vulkan.hpp>
#include "program/layout/DescriptorCount.h"
#include "program/layout/DescriptorBindingLayout.h"

namespace vkr {

/**
 * @brief	Wraps Vulkan DescriptorBindingLayout data.
 */
class DescriptorSetLayout {
public:
	/**
	 * @brief	Default constructor.
	 *
	 * @param	device					Logical device.
	 * @param	binding_initializers	Structures used to populate DescriptorBindingLayouts.
	 */
	DescriptorSetLayout(const vk::Device& device, const std::vector<internal::DescriptorBindingInitializer>& binding_intializers);

	/**
	 * @brief	Retrieve DescriptorBindingLayout with the given binding index.
	 *
	 * @param	binding	Binding index.
	 * @return	Pointer to the DescriptorBindingLayout or nullptr if the DescriptorBindingLayout with the give binding index does not exist.
	 */
	const DescriptorBindingLayout* getDescriptorBinding(uint32_t binding) const;

	/**
	 * @brief	Get DescriptorSetLayout
	 */
	const vk::DescriptorSetLayout& getVkHandle() const;

	/**
	 * @brief	Returns DescriptorCount object that contains descriptor type counters.
	 *
	 * @return	DescriptorCount object.
	 */
	const DescriptorsCount& getDescriptorsCount() const;

	/**
	 * @brief	Free Vulkan resources.
	 */
	~DescriptorSetLayout();

protected:
	/**
	 * @brief	Increment descriptor count of a given type.
	 *
	 * @param	type	Descriptor type.
	 */
	void incrementDescriptorCount(const vk::DescriptorType& type);

private:
	vk::Device device_;													///< Logical device.
	vk::DescriptorSetLayout vk_handle_;									///< Vulkan descriptor set layout handle.
	std::vector<std::unique_ptr<DescriptorBindingLayout>> bindings_;	///< Descriptor bindings in this set (bindings may be defined sparsely).
	DescriptorsCount descriptors_count_;								///< Descriptor count
};

}	///!	namespace vkr

#endif ///!	HEADER_GUARD