#ifndef MEMORY_FRAMEBUFFER_H
#define MEMORY_FRAMEBUFFER_H

#include <logi/program/render_pass/RenderPass.h>
#include <vulkan/vulkan.hpp>
#include "logi/base/Handle.h"
#include "logi/base/ManagedResource.h"
#include "logi/memory/ImageView.h"

namespace logi {

class LogicalDevice;

class Framebuffer : public DestroyableOwnedHandle<LogicalDevice> {
 public:
  /**
   * @brief Default placeholder constructor.
   */
  Framebuffer() = default;

  Framebuffer(const LogicalDevice& device, const RenderPass& render_pass, const std::vector<ImageView>& attachments,
              uint32_t width, uint32_t height, uint32_t layers, const vk::FramebufferCreateFlags& flags);

  const vk::Framebuffer& getVkHandle() const;

  operator vk::Framebuffer() const;

 protected:
  void free() override;

 private:
  using ManagedVkFramebuffer =
    ManagedResource<vk::Device, vk::Framebuffer, vk::DispatchLoaderStatic, &vk::Device::destroyFramebuffer>;

  struct FramebufferData {
    FramebufferData(ManagedVkFramebuffer vk_framebuffer, std::vector<ImageView> attachments, const uint32_t width,
                    const uint32_t height, const uint32_t layers, const vk::FramebufferCreateFlags& flags);

    ManagedVkFramebuffer vk_framebuffer;
    std::vector<ImageView> attachments;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
    vk::FramebufferCreateFlags flags;
  };

  std::shared_ptr<FramebufferData> data_;
};

} // namespace logi

#endif
