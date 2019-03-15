#include "logi/memory/Framebuffer.h"
#include <utility>
#include "logi/base/LogicalDevice.h"

namespace logi {

Framebuffer::Framebuffer(const LogicalDevice& device, const RenderPass& render_pass,
                         const std::vector<ImageView>& attachments, uint32_t width, uint32_t height, uint32_t layers,
                         const vk::FramebufferCreateFlags& flags)
  : DestroyableOwnedHandle<LogicalDevice>(device, true), data_(nullptr) {
  std::vector<vk::ImageView> vk_iv_handles;
  vk_iv_handles.reserve(attachments.size());

  for (const ImageView& attachment : attachments) {
    vk_iv_handles.emplace_back(attachment.getVkHandle());
  }

  vk::FramebufferCreateInfo fb_ci;
  fb_ci.renderPass = render_pass;
  fb_ci.attachmentCount = static_cast<uint32_t>(vk_iv_handles.size());
  fb_ci.pAttachments = vk_iv_handles.data();
  fb_ci.width = width;
  fb_ci.height = height;
  fb_ci.layers = layers;
  fb_ci.flags = flags;

  vk::Device vk_device = device;
  data_ = std::make_shared<FramebufferData>(ManagedVkFramebuffer(vk_device, vk_device.createFramebuffer(fb_ci)),
                                            attachments, width, height, layers, flags);
}

const vk::Framebuffer& Framebuffer::getVkHandle() const {
  checkForNullHandleInvocation("Framebuffer", "getVkHandle");
  return data_->vk_framebuffer.get();
}

Framebuffer::operator vk::Framebuffer() const {
  checkForNullHandleInvocation("Framebuffer", "operator vk::Framebuffer()");
  return data_->vk_framebuffer.get();
}

void Framebuffer::free() {
  if (valid()) {
    data_->vk_framebuffer.destroy();
  }
}

Framebuffer::FramebufferData::FramebufferData(ManagedVkFramebuffer vk_framebuffer, std::vector<ImageView> attachments,
                                              const uint32_t width, const uint32_t height, const uint32_t layers,
                                              const vk::FramebufferCreateFlags& flags)
  : vk_framebuffer(vk_framebuffer), attachments(std::move(attachments)), width(width), height(height), layers(layers),
    flags(flags) {}

} // namespace logi
