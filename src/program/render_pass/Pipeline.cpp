#include "logi/program/render_pass/Pipeline.h"

namespace logi {

Pipeline::Pipeline() : DependentDestroyableHandle({}, false), data_(nullptr) {
}

Pipeline::Pipeline(const std::weak_ptr<HandleManager>& owner, const vk::Device& device, const vk::Pipeline& pipeline, const PipelineLayout& layout)
	: DependentDestroyableHandle(owner), data_(std::make_shared<PipelineData>(ManagedVkPipeline(device, pipeline), layout)) { }

const vk::Pipeline& Pipeline::getVkHandle() const {
	return data_->vk_pipeline.get();
}

const PipelineLayout& Pipeline::layout() const {
	return data_->layout;
}

void Pipeline::free() {
	data_->vk_pipeline.destroy();
}

Pipeline::PipelineData::PipelineData(const ManagedVkPipeline& vk_pipeline, const PipelineLayout& layout)
	: vk_pipeline(vk_pipeline), layout(layout)  {
}


}
