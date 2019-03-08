#ifndef MEMORY_IMAGE_VIEW_H
#define MEMORY_IMAGE_VIEW_H

#include <vulkan/vulkan.hpp>
#include <cstdint>
#include "logi/base/ManagedResource.h"
#include "logi/base/Handle.h"

namespace logi {

/**
 * @brief	Contains configuration for ImageView.
 */
struct ImageViewConfiguration {
	/**
	 * @brief	ImageViewConfiguration default constructor.
	 *
	 * @param	view_type			Image view type.
	 * @param	format				Image view element format.
	 * @param	component_mapping	Specifies remapping of color components.
	 * @param	aspect_mask			Specifies which aspect(s) of the image are included in the view.
	 * @param	base_mip_level		First mipmap level accessible to the view.
	 * @param	mip_level_count		Number of mipmap levels (starting from base_mip_level) accessible to the view.
	 * @param	base_array_layer	First array layer accessible to the view.
	 * @param	layer_count			Number of array layers (starting from base_array_layer) accessible to the view.
	 */
	ImageViewConfiguration(vk::ImageViewType view_type = {}, vk::Format format = {}, vk::ComponentMapping component_mapping = {},
	                       vk::ImageAspectFlags aspect_mask = {}, uint32_t base_mip_level = {}, uint32_t mip_level_count = {},
	                       uint32_t base_array_layer = {}, uint32_t layer_count = {});

	vk::ImageViewType view_type;			///< Image view type.
	vk::Format format;						///< Image view element format.
	vk::ComponentMapping component_mapping;	///< Specifies remapping of color components.
	vk::ImageAspectFlags aspect_mask;		///< Specifies which aspect(s) of the image are included in the view.
	uint32_t base_mip_level;				///< First mipmap level accessible to the view.
	uint32_t mip_level_count;				///< Number of mipmap levels (starting from base_mip_level) accessible to the view.
	uint32_t base_array_layer;				///< First array layer accessible to the view.
	uint32_t layer_count;					///< Number of array layers (starting from base_array_layer) accessible to the view.
};

/**
 * @brief	Handle used to access Vulkan ImageView resource.
 */
class ImageView : public DependentDestroyableHandle {
public:
	ImageView();

	/**
	 * @brief	Create handle used to access ImageView resource.
	 *
	 * @param	owner			HandleManager responsible for this handle.
	 * @param	vk_device		Vulkan device handle.
	 * @param	vk_image		Vulkan image handle.
	 * @param	configuration	ImageView configuration.
	 */
	ImageView(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::Image& vk_image, const ImageViewConfiguration& configuration);

	/**
	 * @brief	Retrieve ImageView configuration.
	 *
	 * @return	ImageViewConfiguration object.
	 */
	const ImageViewConfiguration& configuration() const;

	/**
	 * @brief	Retrieve vk::ImageView handle.
	 *
	 * @return	vk::ImageView handle.
	 */
	const vk::ImageView& getVkHandle() const;

protected:
	/**
	 * @brief	Free resources.
	 */
	void free() override;

private:
	using ManagedVkImageView = ManagedResource<vk::Device, vk::ImageView, vk::DispatchLoaderStatic, &vk::Device::destroyImageView>;
	std::shared_ptr<ImageViewConfiguration> configuration_;	///< ImageView configuration.
	std::shared_ptr<ManagedVkImageView> vk_image_view_;		///< ImageView Vulkan handle.
};

}

#endif