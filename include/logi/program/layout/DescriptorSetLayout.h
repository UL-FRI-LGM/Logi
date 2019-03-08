#ifndef PROGRAM_LAYOUT_DESCRIPTOR_SET_LAYOUT_H
#define PROGRAM_LAYOUT_DESCRIPTOR_SET_LAYOUT_H

#include <vulkan/vulkan.hpp>
#include "logi/program/layout/DescriptorCount.h"
#include "logi/program/layout/DescriptorBindingLayout.h"
#include "logi/base/ManagedResource.h"
#include "logi/base/Handle.h"

namespace logi {

/**
 * @brief	Wraps Vulkan DescriptorBindingLayout data.
 */
class DescriptorSetLayout : public Handle {
public:
	/**
	 * @brief	Default constructor.
	 *
	 * @param	device					Logical device.
	 * @param	binding_intializers	Structures used to populate DescriptorBindingLayouts.
	 */
	DescriptorSetLayout(const vk::Device& device, const std::vector<internal::DescriptorBindingInitializer>& binding_intializers);

	/**
	 * @brief	Retrieve DescriptorBindingLayout with the given binding index.
	 *
	 * @param	binding	Binding index.
	 * @return	Pointer to the DescriptorBindingLayout or nullptr if the DescriptorBindingLayout with the give binding index does not exist.
	 */
	const DescriptorBindingLayout& getDescriptorBinding(uint32_t binding) const;

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

protected:
	/**
	 * @brief	Increment descriptor count of a given type.
	 *
	 * @param	type	Descriptor type.
	 */
	void incrementDescriptorCount(const vk::DescriptorType& type) const;

	void free() override;
private:
	using ManagedVkDescriptorSetLayout = ManagedResource<vk::Device, vk::DescriptorSetLayout, vk::DispatchLoaderStatic, &vk::Device::destroyDescriptorSetLayout>;

	std::shared_ptr<ManagedVkDescriptorSetLayout> vk_descriptor_set_layout_ = nullptr;
	std::shared_ptr<std::vector<DescriptorBindingLayout>> bindings_;			///< Descriptor bindings in this set (bindings may be defined sparsely).
	std::shared_ptr<DescriptorsCount> descriptors_count_;						///< Descriptor count
};

}	///!	namespace logi

#endif ///!	HEADER_GUARD