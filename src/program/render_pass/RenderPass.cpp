#include "logi/program/render_pass/RenderPass.h"
#include <utility>
#include "logi/base/LogicalDevice.h"

namespace logi {

RenderPass::RenderPass(const ProgramManager& program_manager, const RenderPassLayout& layout)
  : DestroyableOwnedHandle<ProgramManager>(program_manager, true), data_(std::make_shared<RenderPassData>(layout)) {
  vk::Device vk_device = getOwner<LogicalDevice>();
  data_->vk_render_pass = ManagedVkRenderPass(vk_device, vk_device.createRenderPass(*data_->layout.buildCreateInfo()));
}

vk::Extent2D RenderPass::renderAreaGranularity() const {
  return data_->vk_render_pass.getOwner().getRenderAreaGranularity(data_->vk_render_pass.get());
}

Framebuffer RenderPass::createFramebuffer(const std::vector<ImageView>& attachments, uint32_t width, uint32_t height,
                                          uint32_t layers, const vk::FramebufferCreateFlags& flags) const {
  return HandleGenerator<RenderPass, Framebuffer>::createHandle(
    data_->vk_render_pass.getOwner(), data_->vk_render_pass.get(), attachments, width, height, layers, flags);
}

const vk::RenderPass& RenderPass::getVkHandle() const {
  return data_->vk_render_pass.get();
}

RenderPass::operator vk::RenderPass() const {
  return data_->vk_render_pass.get();
}

GraphicalPipeline RenderPass::addGraphicalPipeline(const vk::Pipeline& pipeline, const PipelineLayout& layout,
                                                   const PipelineState& state) const {
  return HandleGenerator<RenderPass, GraphicalPipeline>::createHandle(data_->vk_render_pass.getOwner(), pipeline,
                                                                      layout, state);
}

RenderPass::RenderPassData::RenderPassData(RenderPassLayout layout) : layout(std::move(layout)) {}

} // namespace logi
