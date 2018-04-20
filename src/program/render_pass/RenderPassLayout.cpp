#include "program/render_pass/RenderPassLayout.h"

namespace vkr {

attachment_index_t RenderPassLayout::addAttachmentDescription(vk::Format format, vk::SampleCountFlagBits samples, vk::AttachmentLoadOp load_op, vk::AttachmentStoreOp store_op, vk::AttachmentLoadOp stencil_load_op, vk::AttachmentStoreOp stencil_store_op, vk::ImageLayout initial_layout, vk::ImageLayout final_layout, const vk::AttachmentDescriptionFlags& flags) {
	if (final_layout == vk::ImageLayout::eUndefined || final_layout == vk::ImageLayout::ePreinitialized) {
		throw std::runtime_error("Final layout must not be eUndfined or ePreinitialized.");
	}
	
	attachment_descriptions_.emplace_back(std::make_unique<vk::AttachmentDescription>(flags, format, samples, load_op, store_op, stencil_load_op, stencil_store_op, initial_layout, final_layout));
	return attachment_descriptions_.size() - 1;
}

void RenderPassLayout::popAttachmentDescription() {
	attachment_descriptions_.pop_back();
}

const vk::AttachmentDescription& RenderPassLayout::getAttachmentDescription(attachment_index_t index) const {
	return *attachment_descriptions_[index];
}

attachment_index_t RenderPassLayout::attachmentDescriptionCount() const {
	return attachment_descriptions_.size();
}

std::tuple<subpass_index_t, SubpassLayout*> RenderPassLayout::addSubpassLayout() {
	subpasses_.emplace_back(std::make_unique<SubpassLayout>(attachment_descriptions_));
	return std::make_tuple(subpasses_.size() - 1, subpasses_.back().get());
}

SubpassLayout* RenderPassLayout::getSubpass(subpass_index_t id) {
	return subpasses_[id].get();
}

void RenderPassLayout::popSubpass() {
	subpasses_.pop_back();

	// Remove all dependencies concerning this subpass
	for (auto it = subpass_dependencies_.cbegin(); it != subpass_dependencies_.cend();) {
		if (it->second.srcSubpass >= subpasses_.size() || it->second.dstSubpass >= subpasses_.size()) {
			it = subpass_dependencies_.erase(it);
		}
		else {
			++it;
		}
	}
}

dependency_id_t RenderPassLayout::addSubpassDependency(const SubpassLayout* src_subpass, const SubpassLayout* dst_subpass, const vk::PipelineStageFlags& src_stage_mask, const vk::PipelineStageFlags& dst_stage_mask, const vk::AccessFlags& src_access_mask, const vk::AccessFlags& dst_access_mask, const vk::DependencyFlags& dependency_flags) {
	subpass_index_t src_subpass_index = subpasses_.size();
	subpass_index_t dst_subpass_index = subpasses_.size();
	
	// Try to find subpasses.
	for (uint32_t i = 0; i < subpasses_.size(); i++) {
		if (src_subpass == subpasses_[i].get()) {
			src_subpass_index = i;
		}
		if (dst_subpass == subpasses_[i].get()) {
			dst_subpass_index = i;
		}
	}

	// Check if the subpasses were found.
	if (src_subpass_index == subpasses_.size() || dst_subpass_index == subpasses_.size()) {
		throw std::runtime_error("Given subpasses are not a part of this render pass.");
	}

	return addSubpassDependency(src_subpass_index, dst_subpass_index, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags);
}

dependency_id_t RenderPassLayout::addSubpassDependency(subpass_index_t src_subpass_index, subpass_index_t dst_subpass_index, const vk::PipelineStageFlags& src_stage_mask, const vk::PipelineStageFlags& dst_stage_mask, const vk::AccessFlags& src_access_mask, const vk::AccessFlags& dst_access_mask, const vk::DependencyFlags& dependency_flags) {
	static dependency_id_t id_generator = 0;
	dependency_id_t id = id_generator++;

	subpass_dependencies_[id] = vk::SubpassDependency(src_subpass_index, dst_subpass_index, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags);
	return id;
}

const vk::SubpassDependency& RenderPassLayout::getSubpassDependency(dependency_id_t id) const {
	return subpass_dependencies_.at(id);
}

void RenderPassLayout::removeSubpassDependency(dependency_id_t id) {
	subpass_dependencies_.erase(id);
}

void RenderPassLayout::validate() const {
}

vk::RenderPass RenderPassLayout::buildRenderPass(vk::Device device) const {
	validate();

	// Fetch attachments.
	std::vector<vk::AttachmentDescription> attachments_ci{};
	attachments_ci.reserve(attachment_descriptions_.size());
	for (const auto& description : attachment_descriptions_) {
		attachments_ci.push_back(*description);
	}

	// Fetch subpasses.
	std::vector<vk::SubpassDescription> subpasses_ci{};
	subpasses_ci.reserve(subpasses_.size());
	for (const auto& subpass : subpasses_) {
		subpasses_ci.push_back(subpass->getVkSubpassDescription());
	}

	// Fetch dependencies
	std::vector<vk::SubpassDependency> subpass_dependecies_ci{};
	subpass_dependecies_ci.reserve(subpass_dependencies_.size());
	for (const auto& dependency : subpass_dependencies_) {
		subpass_dependecies_ci.push_back(dependency.second);
	}

	// Assemble create info.
	vk::RenderPassCreateInfo render_pass_ci{};
	render_pass_ci.attachmentCount = attachment_descriptions_.size();
	render_pass_ci.pAttachments = &attachments_ci[0];
	render_pass_ci.subpassCount = subpasses_ci.size();
	render_pass_ci.pSubpasses = &subpasses_ci[0];
	render_pass_ci.dependencyCount = subpass_dependecies_ci.size();
	render_pass_ci.pDependencies = &subpass_dependecies_ci[0];

	return device.createRenderPass(render_pass_ci);
}



}
