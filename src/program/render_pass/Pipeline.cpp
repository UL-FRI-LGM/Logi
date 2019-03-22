#include "logi/program/render_pass/Pipeline.h"
#include <utility>
#include "logi/base/LogicalDevice.h"

namespace logi {

Pipeline::Pipeline(const ProgramManager& program_manager, const vk::Pipeline& pipeline, const PipelineLayout& layout)
  : DestroyableOwnedHandle<ProgramManager>(program_manager, true),
    data_(std::make_shared<PipelineData>(
      ManagedVkPipeline(static_cast<vk::Device>(getOwner<LogicalDevice>()), pipeline), layout)) {}

const vk::Pipeline& Pipeline::getVkHandle() const {
  return data_->vk_pipeline.get();
}

const PipelineLayout& Pipeline::layout() const {
  return data_->layout;
}

void Pipeline::free() {
  data_->vk_pipeline.destroy();
}

Pipeline::PipelineData::PipelineData(ManagedVkPipeline vk_pipeline, PipelineLayout layout)
  : vk_pipeline(std::move(vk_pipeline)), layout(std::move(layout)) {}

} // namespace logi
