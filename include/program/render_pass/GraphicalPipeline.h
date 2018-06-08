#ifndef PROGRAM_RENDER_PASS_GRAPHICAL_PIPELINE_H
#define PROGRAM_RENDER_PASS_GRAPHICAL_PIPELINE_H

#include <vulkan/vulkan.hpp>
#include <memory>
#include "program/layout/PipelineLayout.h"
#include "program/layout/PipelineState.h"

namespace logi {

class GraphicalPipeline : public DependentDestroyableHandle {
public:
	GraphicalPipeline();

	GraphicalPipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Pipeline& pipeline, const PipelineLayout& layout, const PipelineState& state);

	const vk::Pipeline& getVkHandle() const;

	const PipelineLayout& layout() const;

	const PipelineState& state() const;

protected:
	void free() override;

private:
	using ManagedVkPipeline = ManagedResource<vk::Device, vk::Pipeline, vk::DispatchLoaderStatic, &vk::Device::destroyPipeline>;

	struct GraphicalPipelineData {
		/**
		 * @brief	Initializes GraphicalPipelineData members with the given data.
		 *
		 * @param	vk_pipeline		Vulkan pipeline handle.
		 * @param	layout			Pipeline layout.
		 * @param	state			Pipeline state.
		 */
		GraphicalPipelineData(const ManagedVkPipeline& vk_pipeline, const PipelineLayout& layout, const PipelineState& state);

		ManagedVkPipeline vk_pipeline;	///< Vulkan pipeline handle.
		PipelineLayout layout;			///< Pipeline layout.
		PipelineState state;			///< Pipeline state.
	};

	std::shared_ptr<GraphicalPipelineData> data_;
};

} ///!	namespace logi

#endif ///! HEADER_GUARD