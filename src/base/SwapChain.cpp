/*
 * Vulkan Renderer base class.
 *
 * Copyright (C) 2017 by Primoz Lavric
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "logi/base/SwapChain.h"
#include <utility>
#include "logi/base/LogicalDevice.h"
#include "logi/base/PhysicalDevice.h"

namespace logi {

SwapChain::SwapChain(const LogicalDevice& device, const vk::SurfaceKHR& surface, const uint32_t present_family,
                     const std::vector<uint32_t>& concurrent_families)
  : DestroyableOwnedHandle<LogicalDevice>(device, true),
    data_(std::make_shared<SwapchainData>(surface, present_family, concurrent_families)) {
  vk::PhysicalDevice vk_physical_device = getOwner<PhysicalDevice>();

  // Check if family with present support was found.
  const bool present_support = vk_physical_device.getSurfaceSupportKHR(present_family, surface);
  if (!present_support) {
    throw std::runtime_error("Given present family does not support presentation on the given surface.");
  }

  // Get list of supported surface formats
  std::vector<vk::SurfaceFormatKHR> surface_formats = vk_physical_device.getSurfaceFormatsKHR(surface);

  if (surface_formats.empty()) {
    throw std::runtime_error("Failed to retrieve supported surface formats.");
  }

  // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED, there is no preferred format, so we
  // assume VK_FORMAT_B8G8R8A8_UNORM.
  if ((surface_formats.size() == 1) && (surface_formats[0].format == vk::Format::eUndefined)) {
    data_->color_format = vk::Format::eB8G8R8A8Unorm;
    data_->color_space = vk::ColorSpaceKHR(surface_formats[0].colorSpace);
  } else {
    // Try to find the preferred VK_FORMAT_B8G8R8A8_UNORM format.
    bool found_B8G8R8A8_UNORM = false;
    for (auto& surface_format : surface_formats) {
      if (surface_format.format == vk::Format::eB8G8R8A8Unorm) {
        data_->color_format = surface_format.format;
        data_->color_space = surface_format.colorSpace;
        found_B8G8R8A8_UNORM = true;
        break;
      }
    }

    // If VK_FORMAT_B8G8R8A8_UNORM format is not available select the first available color format.
    if (!found_B8G8R8A8_UNORM) {
      data_->color_format = surface_formats[0].format;
      data_->color_space = surface_formats[0].colorSpace;
    }
  }
};

void SwapChain::create(const vk::Extent2D& desired_extent, const bool vsync) const {
  vk::PhysicalDevice vk_physical_device = getOwner<PhysicalDevice>();
  vk::Device vk_device = getOwner<LogicalDevice>();
  const vk::SurfaceKHR& surface = data_->surface;
  const vk::SwapchainKHR old_swapchain = data_->swapchain;

  // Get physical device surface properties and formats.
  const vk::SurfaceCapabilitiesKHR surface_capabilites = vk_physical_device.getSurfaceCapabilitiesKHR(surface);

  // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the SwapChain.
  if (surface_capabilites.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
    // If the surface size is undefined, the size is set to the size of the images requested.
    data_->extent.width = desired_extent.width;
    data_->extent.height = desired_extent.height;
  } else {
    // If the surface size is defined, the swap chain size must match
    data_->extent = surface_capabilites.currentExtent;
  }

  // Select a present mode for the SwapChain.
  // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec. This mode waits for the vertical blank
  // ("v-sync")
  vk::PresentModeKHR swapchain_present_mode = vk::PresentModeKHR::eFifo;

  // If v-sync is not requested, try to find a mailbox mode. It's the lowest latency non-tearing present mode available
  if (!vsync) {
    std::vector<vk::PresentModeKHR> present_modes = vk_physical_device.getSurfacePresentModesKHR(surface);

    for (auto& present_mode : present_modes) {
      if (present_mode == vk::PresentModeKHR::eMailbox) {
        swapchain_present_mode = vk::PresentModeKHR::eMailbox;
        break;
      }
      if (present_mode == vk::PresentModeKHR::eImmediate) {
        swapchain_present_mode = vk::PresentModeKHR::eImmediate;
      }
    }
  }

  // Determine the number of images
  uint32_t num_swapchain_images = surface_capabilites.minImageCount + 1;
  if ((surface_capabilites.maxImageCount > 0) && (num_swapchain_images > surface_capabilites.maxImageCount)) {
    num_swapchain_images = surface_capabilites.maxImageCount;
  }

  // Find the transformation of the surface
  vk::SurfaceTransformFlagBitsKHR pre_transform;
  if (surface_capabilites.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
    // We prefer a non-rotated transform
    pre_transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
  } else {
    pre_transform = surface_capabilites.currentTransform;
  }

  // Find a supported composite alpha format (not all devices support alpha opaque)
  static const std::vector<vk::CompositeAlphaFlagBitsKHR> alpha_preferred = {
    vk::CompositeAlphaFlagBitsKHR::eOpaque, vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
    vk::CompositeAlphaFlagBitsKHR::ePostMultiplied, vk::CompositeAlphaFlagBitsKHR::eInherit};

  vk::CompositeAlphaFlagBitsKHR composite_alpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

  // Simply select the first composite alpha format available
  for (auto& composite_alpha_flags : alpha_preferred) {
    if (surface_capabilites.supportedCompositeAlpha & composite_alpha_flags) {
      composite_alpha = composite_alpha_flags;
      break;
    };
  }

  // Swapchain creation
  vk::SwapchainCreateInfoKHR swapchain_ci = {};
  swapchain_ci.pNext = nullptr;
  swapchain_ci.surface = surface;
  swapchain_ci.minImageCount = num_swapchain_images;
  swapchain_ci.imageFormat = data_->color_format;
  swapchain_ci.imageColorSpace = data_->color_space;
  swapchain_ci.imageExtent = data_->extent;
  swapchain_ci.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  swapchain_ci.preTransform = pre_transform;
  swapchain_ci.imageArrayLayers = 1;

  if (data_->concurrent_families.size() == 1u) {
    swapchain_ci.imageSharingMode = vk::SharingMode::eExclusive;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = nullptr;
  } else {
    swapchain_ci.imageSharingMode = vk::SharingMode::eConcurrent;
    swapchain_ci.queueFamilyIndexCount = static_cast<uint32_t>(data_->concurrent_families.size());
    swapchain_ci.pQueueFamilyIndices = data_->concurrent_families.data();
  }

  swapchain_ci.presentMode = swapchain_present_mode;
  swapchain_ci.oldSwapchain = old_swapchain;
  // Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
  swapchain_ci.clipped = true;
  swapchain_ci.compositeAlpha = composite_alpha;

  // Set additional usage flag for blitting from the swapchain images if supported
  const vk::FormatProperties format_properties = vk_physical_device.getFormatProperties(data_->color_format);

  if ((format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrcKHR) ||
      (format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc)) {
    swapchain_ci.imageUsage |= vk::ImageUsageFlagBits::eTransferSrc;
  }

  // Create SwapChain.
  data_->swapchain = vk_device.createSwapchainKHR(swapchain_ci);

  // If an existing swap chain is re-created, destroy the old swap chain
  // This also cleans up all the presentable images
  if (old_swapchain) {
    // Destroy old image views.
    HandleGenerator<SwapChain, Image>::destroyAllHandles();
    vk_device.destroySwapchainKHR(old_swapchain);
  }

  data_->images = vk_device.getSwapchainImagesKHR(data_->swapchain);

  // Get the swap chain buffers containing the image and imageview
  data_->image_views.clear();
  for (const vk::Image& image : data_->images) {
    ImageViewConfiguration iv_config{};
    iv_config.format = data_->color_format;
    iv_config.component_mapping = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB,
                                   vk::ComponentSwizzle::eA};
    iv_config.aspect_mask = vk::ImageAspectFlagBits::eColor;
    iv_config.base_mip_level = 0;
    iv_config.mip_level_count = 1;
    iv_config.base_array_layer = 0;
    iv_config.layer_count = 1;
    iv_config.view_type = vk::ImageViewType::e2D;

    data_->image_views.push_back(image_view_hm->createHandle<ImageView>(device, image, iv_config));
  }

  // Set image index out of range.
  data_->acquired_image_index = static_cast<uint32_t>(data_->image_views.size());
}

void SwapChain::free() {
  if (data_->swapchain) {
    image_view_hm->destroyAllHandles();
    data_->device.destroySwapchainKHR(data_->swapchain);
  }

  DependentDestroyableHandle::free();
}

vk::ResultValue<uint32_t> SwapChain::acquireNextImage(const Semaphore& present_complete_semaphore) const {
  const vk::ResultValue<uint32_t> index =
    data_->device.acquireNextImageKHR(data_->swapchain, UINT64_MAX, present_complete_semaphore.getVkHandle(), nullptr);

  // If image was successfully acqiured, store image index.
  if (index.result == vk::Result::eSuccess) {
    data_->acquired_image_index = index.value;
  }

  return index;
}

const std::vector<ImageView>& SwapChain::getImageViews() const {
  return data_->image_views;
}

vk::Result SwapChain::queuePresent(const Queue& queue, const Semaphore& wait_semaphore) const {
  if (data_->acquired_image_index >= data_->image_views.size()) {
    throw std::runtime_error("Tried to get ImageView before successfully acquiring image.");
  }

  vk::PresentInfoKHR present_info{};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &data_->swapchain;
  present_info.pImageIndices = &data_->acquired_image_index;
  // Check if a wait semaphore has been specified to wait for before presenting the image
  if (wait_semaphore) {
    present_info.pWaitSemaphores = &wait_semaphore.getVkHandle();
    present_info.waitSemaphoreCount = 1;
  }

  return queue.getVkHandle().presentKHR(present_info);
}

vk::Format SwapChain::getColorFormat() const {
  return data_->color_format;
}

const vk::Extent2D& SwapChain::getExtent() const {
  return data_->extent;
}

SwapChain::SwapchainData::SwapchainData(const vk::SurfaceKHR& surface, uint32_t present_family,
                                        std::vector<uint32_t> concurrent_families)
  : surface(surface), present_family(present_family), concurrent_families(std::move(concurrent_families)) {
  // If present family is not among concurrent families add it.
  if (std::find(this->concurrent_families.begin(), this->concurrent_families.end(), present_family) ==
      this->concurrent_families.end()) {
    this->concurrent_families.emplace_back(present_family);
  }
}

} // namespace logi
