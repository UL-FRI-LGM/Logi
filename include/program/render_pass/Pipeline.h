#ifndef PROGRAM_RENDER_PASS_PIPELINE_H
#define PROGRAM_RENDER_PASS_PIPELINE_H

#include <vulkan/vulkan.hpp>
#include <memory>
#include "program/layout/PipelineLayout.h"

namespace logi {

class Pipeline : public DependentDestroyableHandle {
public:

	Pipeline();

	Pipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Pipeline& pipeline, const PipelineLayout& layout);

	const vk::Pipeline& getVkHandle() const;

	const PipelineLayout& layout() const;

protected:
	void free() override;

	
private:
	using ManagedVkPipeline = ManagedResource<vk::Device, vk::Pipeline, vk::DispatchLoaderStatic, &vk::Device::destroyPipeline>;

	struct PipelineData {
		/**
		* @brief	Initializes GraphicalPipelineData members with the given data.
		*
		* @param	vk_pipeline		Vulkan pipeline handle.
		* @param	layout			Pipeline layout.
		*/
		PipelineData(const ManagedVkPipeline& vk_pipeline, const PipelineLayout& layout);

		ManagedVkPipeline vk_pipeline;	///< Vulkan pipeline handle.
		PipelineLayout layout;			///< Pipeline layout.
	};

	std::shared_ptr<PipelineData> data_;
};

}

#endif