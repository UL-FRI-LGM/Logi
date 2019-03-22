#include <utility>

#ifndef MEMORY_IMAGE_VIEW_H
#define MEMORY_IMAGE_VIEW_H

#include <cstdint>
#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"

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
  explicit ImageViewConfiguration(vk::ImageViewType view_type = {}, vk::Format format = {},
                                  vk::ComponentMapping component_mapping = {},
                                  const vk::ImageAspectFlags& aspect_mask = {}, uint32_t base_mip_level = {},
                                  uint32_t mip_level_count = {}, uint32_t base_array_layer = {},
                                  uint32_t layer_count = {});

  vk::ImageViewType view_type;            ///< Image view type.
  vk::Format format;                      ///< Image view element format.
  vk::ComponentMapping component_mapping; ///< Specifies remapping of color components.
  vk::ImageAspectFlags aspect_mask;       ///< Specifies which aspect(s) of the image are included in the view.
  uint32_t base_mip_level;                ///< First mipmap level accessible to the view.
  uint32_t mip_level_count;  ///< Number of mipmap levels (starting from base_mip_level) accessible to the view.
  uint32_t base_array_layer; ///< First array layer accessible to the view.
  uint32_t layer_count;      ///< Number of array layers (starting from base_array_layer) accessible to the view.
};

class Image;

/**
 * @brief	Handle used to access Vulkan ImageView resource.
 */
class ImageView : public DestroyableOwnedHandle<ImageView, Image> {
 public:
  ImageView() = default;

  /**
   * @brief	Create handle used to access ImageView resource.
   *
   * @param	image		      Image handle.
   * @param	configuration ImageView configuration.
   */
  ImageView(const Image& image, const ImageViewConfiguration& configuration);

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

  /**
   * @brief   vk::ImageView conversion.
   *
   * @return	vk::ImageView
   */
  operator vk::ImageView() const;

 protected:
  /**
   * @brief	Free resources.
   */
  void free() override;

 private:
  using ManagedVkImageView =
    ManagedResource<vk::Device, vk::ImageView, vk::DispatchLoaderStatic, &vk::Device::destroyImageView>;

  struct Data {
    /**
     * @brief Initializes members.
     *
     * @param config          Image view configuration.
     * @param vk_image_view   Image view Vulkan handle.
     */
    Data(ImageViewConfiguration config, ManagedVkImageView vk_image_view);

    /**
     * ImageView configuration.
     */
    ImageViewConfiguration configuration;

    /**
     * ImageView Vulkan handle.
     */
    ManagedVkImageView vk_image_view;
  };

  /**
   * Internal data.
   */
  std::shared_ptr<Data> data_;
};

} // namespace logi

#endif