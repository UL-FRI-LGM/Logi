#include "logi/program/render_pass/RenderPassLayout.h"

namespace logi {

RenderPassLayout::RenderPassLayout(size_t attachment_count, size_t subpass_count) : attachments_(attachment_count, vk::AttachmentDescription()), subpasses_(subpass_count, SubpassLayout()), vk_subpasses_(), dependencies_() {

}

void RenderPassLayout::setAttachmentDescription(attachment_index_t index, const vk::AttachmentDescription& attachment_desc) {
	// Validate index.
	if (index >= attachments_.size()) {
		throw std::runtime_error("Attachment index out of range.");
	}

	attachments_[index] = attachment_desc;
}

vk::AttachmentDescription* RenderPassLayout::getAttachmentDescription(attachment_index_t index) {
	return &attachments_[index];
}

void RenderPassLayout::setSubpassLayout(subpass_index_t index, const SubpassLayout& subpass_layout) {
	// Validate index.
	if (index >= subpasses_.size()) {
		throw std::runtime_error("Subpass index out of range.");
	}

	subpasses_[index] = subpass_layout;
}

SubpassLayout* RenderPassLayout::getSubpassLayout(subpass_index_t index) {
	return &subpasses_[index];
}

void RenderPassLayout::addDependency(subpass_index_t src, subpass_index_t dst, vk::PipelineStageFlags src_stage_mask, vk::PipelineStageFlags dst_stage_mask, vk::AccessFlags src_access_mask, vk::AccessFlags dst_access_mask, vk::DependencyFlags dependency_flags) {
	// Validate indices.
	if (src >= subpasses_.size() && src != VK_SUBPASS_EXTERNAL || dst >= subpasses_.size() && dst != VK_SUBPASS_EXTERNAL) {
		throw std::runtime_error("Sub pass dependency indices out of range.");
	}
	
	// Try to find existing dependencies.
	auto it = std::find_if(dependencies_.begin(), dependencies_.end(), [src, dst](const vk::SubpassDependency& dependency) {
		return dependency.srcSubpass == src && dependency.dstSubpass == dst;
	});

	// If subpass dependency already exists, update it.
	if (it != dependencies_.end()) {
		it->srcStageMask = src_stage_mask;
		it->dstStageMask = dst_stage_mask;
		it->srcAccessMask = src_access_mask;
		it->dstAccessMask = dst_access_mask;
		it->dependencyFlags = dependency_flags;
		return;
	}

	dependencies_.emplace_back(vk::SubpassDependency(src, dst, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags));
}

vk::SubpassDependency* RenderPassLayout::getDependency(subpass_index_t src, subpass_index_t dst) {
	// Try to find dependency between src and dst sub pass.
	auto it = std::find_if(dependencies_.begin(), dependencies_.end(), [src, dst](const vk::SubpassDependency& dependency) {
		return dependency.srcSubpass == src && dependency.dstSubpass == dst;
	});

	// If not found return nullptr.
	if (it == dependencies_.end()) {
		return nullptr;
	}

	return &(*it);
}

void RenderPassLayout::removeDependency(subpass_index_t src, subpass_index_t dst) {
	// Try to find dependency between src and dst sub pass.
	auto it = std::find_if(dependencies_.begin(), dependencies_.end(), [src, dst](const vk::SubpassDependency& dependency) {
		return dependency.srcSubpass == src && dependency.dstSubpass == dst;
	});

	// If found remove it.
	if (it != dependencies_.end()) {
		dependencies_.erase(it);
	}
}

vk::RenderPassCreateInfo* RenderPassLayout::buildCreateInfo() {
	// Create SubpassDescriptions from SubpassLayouts.
	vk_subpasses_.clear();
	for (SubpassLayout& layout : subpasses_) {
		vk_subpasses_.emplace_back(layout.createVkSubpassDescription());
	}

	// Build create info.
	vk_create_info_ = vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), 
		static_cast<uint32_t>(attachments_.size()), (attachments_.empty()) ? nullptr : &attachments_[0],
		static_cast<uint32_t>(vk_subpasses_.size()), (vk_subpasses_.empty()) ? nullptr : &vk_subpasses_[0],
		static_cast<uint32_t>(dependencies_.size()), (dependencies_.empty()) ? nullptr : &dependencies_[0]);

	return &vk_create_info_;
}
}
