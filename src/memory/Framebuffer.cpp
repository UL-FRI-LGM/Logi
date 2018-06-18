#include <utility>
#include "memory/Framebuffer.h"

namespace logi {


Framebuffer::Framebuffer() : DependentDestroyableHandle({}, false), vk_framebuffer(nullptr), data_(nullptr) {}

Framebuffer::Framebuffer(std::weak_ptr<HandleManager> owner, const vk::Device device, const vk::RenderPass& render_pass,
	const std::vector<ImageView>& attachments, uint32_t width, uint32_t height, uint32_t layers,
	const vk::FramebufferCreateFlags& flags)
	: DependentDestroyableHandle(owner), vk_framebuffer(nullptr), data_(std::make_shared<FramebufferData>(attachments, width, height, layers, flags)) {
	
	std::vector<vk::ImageView> vk_iv_handles;
	vk_iv_handles.reserve(attachments.size());

	for (const ImageView& attachment : attachments) {
		vk_iv_handles.emplace_back(attachment.getVkHandle());
	}

	vk::FramebufferCreateInfo fb_ci;
	fb_ci.renderPass = render_pass;
	fb_ci.attachmentCount = vk_iv_handles.size();
	fb_ci.pAttachments = vk_iv_handles.data();
	fb_ci.width = width;
	fb_ci.height = height;
	fb_ci.layers = layers;
	fb_ci.flags = flags;

	vk_framebuffer = std::make_shared<ManagedVkFramebuffer>(device, device.createFramebuffer(fb_ci));
}

const vk::Framebuffer& Framebuffer::getVkHandle() const {
	return vk_framebuffer->get();
}

void Framebuffer::free() {
	vk_framebuffer->destroy();
}


Framebuffer::FramebufferData::FramebufferData(std::vector<ImageView> attachments, const uint32_t width,
                                              const uint32_t height, const uint32_t layers, vk::FramebufferCreateFlags flags)
	: attachments(std::move(attachments)), width(width),
	  height(height), layers(layers), flags(std::move(flags)) {}


}
