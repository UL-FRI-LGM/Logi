#include "logi/memory/ImageView.h"
#include "logi/base/LogicalDevice.h"
#include "logi/memory/Image.h"

namespace logi {

ImageViewConfiguration::ImageViewConfiguration(const vk::ImageViewType view_type, const vk::Format format,
                                               const vk::ComponentMapping component_mapping,
                                               const vk::ImageAspectFlags& aspect_mask, const uint32_t base_mip_level,
                                               const uint32_t mip_level_count, const uint32_t base_array_layer,
                                               const uint32_t layer_count)
  : view_type(view_type), format(format), component_mapping(component_mapping), aspect_mask(aspect_mask),
    base_mip_level(base_mip_level), mip_level_count(mip_level_count), base_array_layer(base_array_layer),
    layer_count(layer_count) {}

ImageView::ImageView(const Image& image, const ImageViewConfiguration& configuration)
  : DestroyableOwnedHandle(image, true), data_(nullptr) {
  // Create configuration structures.
  const vk::ImageSubresourceRange sub_resource_range(configuration.aspect_mask, configuration.base_mip_level,
                                                     configuration.mip_level_count, configuration.base_array_layer,
                                                     configuration.layer_count);

  const vk::ImageViewCreateInfo iw_create_info(vk::ImageViewCreateFlags(), image, configuration.view_type,
                                               configuration.format, configuration.component_mapping,
                                               sub_resource_range);

  vk::Device vk_device = getOwner<LogicalDevice>();
  data_ =
    std::make_shared<Data>(configuration, ManagedVkImageView(vk_device, vk_device.createImageView(iw_create_info)));
}

const ImageViewConfiguration& ImageView::configuration() const {
  checkForNullHandleInvocation("ImageView", "configuration");
  return data_->configuration;
}

const vk::ImageView& ImageView::getVkHandle() const {
  checkForNullHandleInvocation("ImageView", "getVkHandle");
  return data_->vk_image_view.get();
}

ImageView::operator vk::ImageView() const {
  checkForNullHandleInvocation("ImageView", "operator vk::ImageView()");
  return data_->vk_image_view.get();
}

void ImageView::free() {
  if (valid()) {
    data_->vk_image_view.destroy();
    DestroyableOwnedHandle::free();
  }
}

ImageView::Data::Data(ImageViewConfiguration config, ImageView::ManagedVkImageView vk_image_view)
  : configuration(std::move(config)), vk_image_view(vk_image_view) {}

} // namespace logi
