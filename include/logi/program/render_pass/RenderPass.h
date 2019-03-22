#ifndef PROGRAM_RENDER_PASS_RENDER_PASS_H
#define PROGRAM_RENDER_PASS_RENDER_PASS_H

#include <vulkan/vulkan.hpp>
#include "logi/memory/Framebuffer.h"
#include "logi/program/layout/PipelineState.h"
#include "logi/program/render_pass/GraphicalPipeline.h"
#include "logi/program/render_pass/RenderPassLayout.h"

namespace logi {

class ProgramManager;

class RenderPass : public DestroyableOwnedHandle<RenderPass, ProgramManager>,
                   public HandleGenerator<RenderPass, GraphicalPipeline>,
                   public HandleGenerator<RenderPass, Framebuffer> {
  friend class ProgramManager;

 public:
  RenderPass() = default;

  RenderPass(const ProgramManager& program_manager, const RenderPassLayout& layout);

  vk::Extent2D renderAreaGranularity() const;

  Framebuffer createFramebuffer(const std::vector<ImageView>& attachments, uint32_t width, uint32_t height,
                                uint32_t layers, const vk::FramebufferCreateFlags& flags = {}) const;

  const vk::RenderPass& getVkHandle() const;

  operator vk::RenderPass() const;

 protected:
  GraphicalPipeline addGraphicalPipeline(const vk::Pipeline& pipeline, const PipelineLayout& layout,
                                         const PipelineState& state) const;

 private:
  using ManagedVkRenderPass =
    ManagedResource<vk::Device, vk::RenderPass, vk::DispatchLoaderStatic, &vk::Device::destroyRenderPass>;

  struct RenderPassData {
    explicit RenderPassData(RenderPassLayout layout);

    ManagedVkRenderPass vk_render_pass{};
    RenderPassLayout layout;
  };

  std::shared_ptr<RenderPassData> data_;
};

} // namespace logi

#endif