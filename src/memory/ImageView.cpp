#include "memory/ImageView.h"

namespace vkr {

ImageView::ImageView(const std::shared_ptr<ManagedVkImageView>& vk_image_view, vk::Format format, const vk::ComponentMapping& component_mapping, const vk::ImageAspectFlags& aspect_mask, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_array_layer, uint32_t layer_count)
	: vk_image_view_(vk_image_view), format(format), component_mapping(component_mapping), aspect_mask(aspect_mask), base_mip_level(base_mip_level), mip_level_count(mip_level_count), base_array_layer(base_array_layer), layer_count(layer_count) { }

const vk::ImageView& ImageView::getVkHandle() const {
	return vk_image_view_->get();
}

void ImageView::destroy() const {
	vk_image_view_->destroy();
}

}
