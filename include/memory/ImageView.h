#ifndef MEMORY_IMAGE_VIEW_H
#define MEMORY_IMAGE_VIEW_H

#include <vulkan/vulkan.hpp>
#include "base/ManagedResource.h"

namespace vkr {

using ManagedVkImageView = ManagedResource<vk::Device, vk::ImageView, vk::DispatchLoaderStatic, &vk::Device::destroyImageView>;

struct ImageView {
	/**
	 * @brief	Handle used to access ImageView resource.
	 *
	 * @param	vk_image_view		Vulkan image view managed object.
	 * @param	format				Image view element format.
	 * @param	component_mapping	Specifies a remapping of color components.
	 * @param	aspect_mask			Specifies which aspect(s) of the image are included in the view.
	 * @param	base_mip_level		First mipmap level accessible to the view.
	 * @param	mip_level_count		Number of mipmap levels (starting from base_mip_level) accessible to the view.
	 * @param	base_array_layer	First array layer accessible to the view.
	 * @param	layer_count			Number of array layers (starting from base_array_layer) accessible to the view.
	 */
	ImageView(const std::shared_ptr<ManagedVkImageView>& vk_image_view, vk::Format format, const vk::ComponentMapping& component_mapping, const vk::ImageAspectFlags& aspect_mask, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_array_layer, uint32_t layer_count);

	/**
	* @brief	Retrieve vk::ImageView handle.
	*
	* @return	vk::ImageView handle.
	*/
	const vk::ImageView& getVkHandle() const;

	/**
	* @brief	Free resources.
	*/
	void destroy() const;

	const vk::Format format;						///< Image view element format.
	const vk::ComponentMapping component_mapping;	///< Specifies a remapping of color components.
	const vk::ImageAspectFlags aspect_mask;			///< Specifies which aspect(s) of the image are included in the view.
	const uint32_t base_mip_level;					///< First mipmap level accessible to the view.
	const uint32_t mip_level_count;					///< Number of mipmap levels (starting from base_mip_level) accessible to the view.
	const uint32_t base_array_layer;				///< First array layer accessible to the view.
	const uint32_t layer_count;						///< Number of array layers (starting from base_array_layer) accessible to the view.
private:
	std::shared_ptr<ManagedVkImageView> vk_image_view_;
};

}

#endif