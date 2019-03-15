/*
 * Vulkan Renderer base class.
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SWAPCHAIN_H_
#define SWAPCHAIN_H_

#include <vulkan/vulkan.hpp>
#include "logi/memory/ImageView.h"
#include "logi/queues/Queue.h"
#include "logi/synchronization/Semaphore.h"

namespace logi {

class SwapChain : public DestroyableOwnedHandle<LogicalDevice> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  SwapChain() = default;

  /**
   * @brief Default constructor that initializes SwapChain members to default values.
   */
  SwapChain(const LogicalDevice& device, const vk::SurfaceKHR& surface, uint32_t present_family,
            const std::vector<uint32_t>& concurrent_families);

  /**
   * @brief Create the SwapChain and retrieve images with the given width and height. Width and height may change to
   * meet the surface requirements.
   *
   * @param desired_extent Requested SwapChain extent.
   * @param vsync Should v-sync be enabled.
   */
  void create(const vk::Extent2D& desired_extent, bool vsync = false) const;

  /**
   * @brief Acquires the next image in the swap chain.
   * @note The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX.
   *
   * @param present_complete_semaphore (Optional) Semaphore that is signaled when the image is ready for use.
   * @return Tuple containing the result of the operation and index of the next image (valid if the operation was
   * successful)
   */
  vk::ResultValue<uint32_t> acquireNextImage(const Semaphore& present_complete_semaphore = {}) const;

  const std::vector<ImageView>& getImageViews() const;

  /**
   * @brief Queue an image for presentation.
   *
   * @param queue Presentation queue for presenting the image.
   * @param image_index Index of the swap chain image to queue for presentation.
   * @param wait_semaphore (Optional) Semaphore that is waited on before the image is presented.
   *
   * @return Result of the queue presentation.
   */
  vk::Result queuePresent(const Queue& queue, const Semaphore& wait_semaphore = {}) const;

  vk::Format getColorFormat() const;

  const vk::Extent2D& getExtent() const;

 protected:
  /**
   * @brief Clean up swap chain data, surface data and disconnect from the device.
   */
  void free() override;

 private:
  struct SwapchainData {
    SwapchainData(const vk::SurfaceKHR& surface, uint32_t present_family, std::vector<uint32_t> concurrent_families);

    vk::SurfaceKHR surface; ///< Surface.
    uint32_t present_family;
    std::vector<uint32_t> concurrent_families;

    vk::SwapchainKHR swapchain{};
    vk::Extent2D extent{};
    vk::Format color_format{};       ///< Selected swap chain color format.
    vk::ColorSpaceKHR color_space{}; ///< Selected swap chain color space.

    std::vector<vk::Image> images{};      ///< Swap chain images.
    std::vector<ImageView> image_views{}; ///< Views of swap chain images
    uint32_t acquired_image_index{};
  };

  std::shared_ptr<SwapchainData> data_;
};

} // namespace logi
#endif