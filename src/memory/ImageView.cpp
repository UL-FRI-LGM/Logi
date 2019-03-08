#include "logi/memory/ImageView.h"

namespace logi {

ImageViewConfiguration::ImageViewConfiguration(const vk::ImageViewType view_type, const vk::Format format, const vk::ComponentMapping component_mapping,
											   vk::ImageAspectFlags aspect_mask, const uint32_t base_mip_level,
											   const uint32_t mip_level_count, const uint32_t base_array_layer, 
											   const uint32_t layer_count)
	: view_type(view_type), format(format), component_mapping(std::move(component_mapping)), aspect_mask(std::move(aspect_mask)),
	base_mip_level(base_mip_level), mip_level_count(mip_level_count), base_array_layer(base_array_layer), layer_count(layer_count) {}


ImageView::ImageView() : DependentDestroyableHandle({}, false) {
}

ImageView::ImageView(const std::weak_ptr<HandleManager>& owner, const vk::Device& vk_device, const vk::Image& vk_image, const ImageViewConfiguration& configuration)
	: DependentDestroyableHandle(owner), configuration_(std::make_shared<ImageViewConfiguration>(configuration)), vk_image_view_(nullptr) {

	// Create configuration structures.
	const vk::ImageSubresourceRange subresource_range(configuration_->aspect_mask, configuration_->base_mip_level, configuration_->mip_level_count, 
												configuration_->base_array_layer, configuration_->layer_count);

	const vk::ImageViewCreateInfo iw_create_info(vk::ImageViewCreateFlags(), vk_image, configuration_->view_type, configuration_->format,
										   configuration_->component_mapping, subresource_range);

	vk_image_view_ = std::make_shared<ManagedVkImageView>(vk_device, vk_device.createImageView(iw_create_info));
}

const ImageViewConfiguration& ImageView::configuration() const {	
	return *configuration_;
}


const vk::ImageView& ImageView::getVkHandle() const {
	if (alive()) {
		return vk_image_view_->get();
	}

	throw std::runtime_error("Called 'getVkHandle' on destroyed ImageView object.");
}

void ImageView::free() {
	vk_image_view_->destroy();
	Handle::free();
}


}
