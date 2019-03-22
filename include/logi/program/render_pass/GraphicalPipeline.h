#ifndef PROGRAM_RENDER_PASS_GRAPHICAL_PIPELINE_H
#define PROGRAM_RENDER_PASS_GRAPHICAL_PIPELINE_H

#include <memory>
#include <vulkan/vulkan.hpp>
#include "logi/program/layout/PipelineLayout.h"
#include "logi/program/layout/PipelineState.h"
#include "logi/program/render_pass/Pipeline.h"

namespace logi {

class GraphicalPipeline : public Pipeline {
 public:
  GraphicalPipeline() = default;

  GraphicalPipeline(const ProgramManager& program_manager, const vk::Pipeline& pipeline, const PipelineLayout& layout,
                    const PipelineState& state);

  const PipelineState& state() const;

 private:
  std::shared_ptr<PipelineState> state_;
};

} // namespace logi

#endif ///! HEADER_GUARD