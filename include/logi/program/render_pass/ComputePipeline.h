#ifndef PROGRAM_RENDER_PASS_COMPUTE_PIPELINE_H
#define PROGRAM_RENDER_PASS_COMPUTE_PIPELINE_H

#include <memory>
#include <vulkan/vulkan.hpp>
#include "logi/program/layout/PipelineLayout.h"
#include "logi/program/render_pass/Pipeline.h"

namespace logi {

class ComputePipeline : public Pipeline {
 public:
  ComputePipeline() = default;

  ComputePipeline(const ProgramManager& program_manager, const vk::Pipeline& pipeline, const PipelineLayout& layout);

  const vk::Pipeline& getVkHandle() const;

  const PipelineLayout& layout() const;

 protected:
  void free() override;

 private:
  using ManagedVkPipeline =
    ManagedResource<vk::Device, vk::Pipeline, vk::DispatchLoaderStatic, &vk::Device::destroyPipeline>;

  struct PipelineData {
    /**
     * @brief	Initializes GraphicalPipelineData members with the given data.
     *
     * @param	vk_pipeline		Vulkan pipeline handle.
     * @param	layout			Pipeline layout.
     */
    PipelineData(ManagedVkPipeline vk_pipeline, PipelineLayout layout);

    ManagedVkPipeline vk_pipeline; ///< Vulkan pipeline handle.
    PipelineLayout layout;         ///< Pipeline layout.
  };

  std::shared_ptr<PipelineData> data_;
};

} // namespace logi

#endif