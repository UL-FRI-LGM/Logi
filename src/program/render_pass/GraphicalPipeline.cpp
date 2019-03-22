#include "logi/program/render_pass/GraphicalPipeline.h"

namespace logi {

GraphicalPipeline::GraphicalPipeline(const ProgramManager& program_manager, const vk::Pipeline& pipeline,
                                     const PipelineLayout& layout, const PipelineState& state)
  : Pipeline(program_manager, pipeline, layout), state_(std::make_shared<PipelineState>(state)) {}

const PipelineState& GraphicalPipeline::state() const {
  return *state_;
}

} // namespace logi
