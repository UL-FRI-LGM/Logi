#ifndef PROGRAM_LAYOUT_PIPELINE_LAYOUT_H
#define PROGRAM_LAYOUT_PIPELINE_LAYOUT_H

#include <utility>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "logi/base/ManagedResource.h"
#include "logi/program/layout/DescriptorSetLayout.h"
#include "logi/program/layout/PushConstantRange.h"
#include "logi/program/layout/Shader.h"
#include "logi/program/layout/VertexAttributeLayout.h"

namespace logi {

/**
 * @brief	Used to specify pipeline type that can be either graphical or compute.
 */
enum class PipelineType { eGraphical = 0u, eCompute, eUndefined };

class ProgramManager;

/**
 * @brief	Wraps the data required to create VkPipelineLayout. It retrieves the data from the given shaders by performing
 * shader reflection using spriv-cross.
 */
class PipelineLayout : public DestroyableOwnedHandle<ProgramManager>,
                       public HandleGenerator<PipelineLayout, DescriptorSetLayout> {
 public:
  /**
   * @brief	Default placeholder constructor.
   */
  PipelineLayout() = default;

  /**
   * @brief Creates and initializes PipelineLayout from the given shaders.
   *
   * @param	prog_manager  Program manager.
   * @param	shaders	      Vector of shaders that form a pipeline.
   */
  PipelineLayout(const ProgramManager& prog_manager, const std::vector<PipelineShaderStage>& shaders);

  /**
   * @brief	Retrieve Pipeline type eCompute/eGraphical.
   *
   * @return	Pipeline type.
   */
  PipelineType getPipelineType() const;

  /**
   * @brief	Retrieve vector of Vulkan shader handles built for the given device.
   *
   * @return	Vector containing shader handles.
   */
  std::vector<vk::PipelineShaderStageCreateInfo> getVkShaderHandles() const;

  /**
   * @brief	Retrieve Vulkan layout handle.
   *
   * @return	Pipeline layout.
   */
  const vk::PipelineLayout& getVkHandle() const;

  /**
   * @brief	Retrieve descriptors count for the given pipeline layout.
   *
   * @return	DescriptorsCount object.
   */
  const DescriptorsCount& getDescriptorsCount() const;

  /**
   * @brief	Retrieve the descriptor set layout of the set with the given index.
   *
   * @param	set	Set index.
   * @return	Pointer to DescriptorSetLayout.
   */
  const DescriptorSetLayout& getDescriptorSetLayout(size_t set) const;

  const std::vector<VertexAttributeLayout>& getAttributeLayouts() const;

  /**
   * @brief	Get number of descriptor sets.
   *
   * @return	Descriptor set count.
   */
  size_t getDescriptorSetCount() const;

 protected:
  /**
   * @brief	Perform shader reflection using spriv-cross.
   */
  void shaderReflection(const vk::Device& device) const;

  /**
   * @brief	Builds vulkan pipeline layout.
   */
  void buildVkPipelineLayout(const vk::Device& device);

 private:
  using ManagedVkPipelineLayout =
    ManagedResource<vk::Device, vk::PipelineLayout, vk::DispatchLoaderStatic, &vk::Device::destroyPipelineLayout>;

  struct PipelineLayoutConfig {
    explicit PipelineLayoutConfig(std::vector<PipelineShaderStage> shaders)
      : type(PipelineType::eUndefined), shaders(std::move(shaders)) {}

    PipelineType type;                                ///< Pipeline type (graphical or compute).
    std::vector<PipelineShaderStage> shaders;         ///< Pipeline shaders.
    std::vector<DescriptorSetLayout> descriptor_sets; ///< Descriptor sets.
    std::vector<PushConstantRange> push_constants;    ///< Push constants.
    std::vector<VertexAttributeLayout> attributes;    ///< Only for graphical pipelines.
    DescriptorsCount combined_descriptors_count;      ///< Combined number of descriptors.
  };

  std::shared_ptr<PipelineLayoutConfig> config_;
  std::shared_ptr<ManagedVkPipelineLayout> vk_pipeline_layout_;
};

} // namespace logi

#endif ///! HEADER_GUARD