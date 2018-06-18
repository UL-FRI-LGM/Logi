#ifndef MEMORY_FRAMEBUFFER_H
#define MEMORY_FRAMEBUFFER_H

#include <vulkan/vulkan.hpp>
#include "base/ManagedResource.h"
#include "base/Handle.h"
#include "memory/ImageView.h"

namespace logi {

class Framebuffer : public DependentDestroyableHandle {
public:
	Framebuffer();

	Framebuffer(std::weak_ptr<HandleManager> owner, const vk::Device device, const vk::RenderPass& render_pass, const std::vector<ImageView>& attachments, 
				uint32_t width, uint32_t height, uint32_t layers, const vk::FramebufferCreateFlags& flags);

	const vk::Framebuffer& getVkHandle() const;

protected:
	void free() override;

private:
	using ManagedVkFramebuffer = ManagedResource<vk::Device, vk::Framebuffer, vk::DispatchLoaderStatic, &vk::Device::destroyFramebuffer>;

	struct FramebufferData {
		FramebufferData(std::vector<ImageView> attachments, const uint32_t width, const uint32_t height,
		                const uint32_t layers, vk::FramebufferCreateFlags flags);

		ManagedVkFramebuffer vk_framebuffer;
		std::vector<ImageView> attachments;
		uint32_t width;
		uint32_t height;
		uint32_t layers;
		vk::FramebufferCreateFlags flags;
	};

	std::shared_ptr<ManagedVkFramebuffer> vk_framebuffer;
	std::shared_ptr<FramebufferData> data_;
};


}

#endif
