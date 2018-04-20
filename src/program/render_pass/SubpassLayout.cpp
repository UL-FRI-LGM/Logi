#include "program/render_pass/SubpassLayout.h"

namespace vkr {

SubpassLayout::SubpassLayout(const std::vector<std::unique_ptr<vk::AttachmentDescription>>& attachments, vk::PipelineBindPoint pipeline_bind_point)
	: pipeline_bind_point_(pipeline_bind_point), input_attachments_(), color_attachments_(), resolve_attachments_(), depth_stencil_attachment_(), preserve_attachments_(), attachment_descriptions_(attachments) {
}

void SubpassLayout::addInputAttachment(attachment_index_t index, vk::ImageLayout layout) {
	addOrUpdateAttachment(input_attachments_, index, layout);
}

void SubpassLayout::removeInputAttachment(attachment_index_t index) {
	removeAttachment(input_attachments_, index);
}

void SubpassLayout::clearInputAttachments() {
	input_attachments_.clear();
	vk_subpass_description_.reset();
}

void SubpassLayout::addColorAttachment(attachment_index_t index, vk::ImageLayout layout) {
	addOrUpdateAttachment(color_attachments_, index, layout);
}

void SubpassLayout::removeColorAttachment(attachment_index_t index) {
	removeAttachment(color_attachments_, index);
}

void SubpassLayout::clearColorAttachments() {
	color_attachments_.clear();
	vk_subpass_description_.reset();
}

void SubpassLayout::addResolveAttachment(attachment_index_t index, vk::ImageLayout layout) {
	addOrUpdateAttachment(resolve_attachments_, index, layout);
}

void SubpassLayout::removeResolveAttachment(attachment_index_t index) {
	removeAttachment(resolve_attachments_, index);
}

void SubpassLayout::clearResolveAttachments() {
	resolve_attachments_.clear();
	vk_subpass_description_.reset();
}

void SubpassLayout::setDepthStencilAttachment(attachment_index_t index, vk::ImageLayout layout) {
	depth_stencil_attachment_.emplace(index, layout);
	vk_subpass_description_.reset();
}

void SubpassLayout::unsetDepthStencilAttachment() {
	if (depth_stencil_attachment_.has_value()) {
		depth_stencil_attachment_.reset();
		vk_subpass_description_.reset();
	}
}

void SubpassLayout::preserveAttachments(attachment_index_t index, attachment_index_t indices ...) {
	// Check if the attachment is already among preserve attachments.
	auto it = std::find(preserve_attachments_.begin(), preserve_attachments_.end(), index);

	// Add attachment index if not found.
	if (it != preserve_attachments_.end()) {
		preserve_attachments_.emplace_back(index);
	}
}

void SubpassLayout::dontPreserveAttachments(attachment_index_t index, attachment_index_t indices ...) {
	// Try to find preserved attachment index.
	auto it = std::find(preserve_attachments_.begin(), preserve_attachments_.end(), index);

	// Remove if found.
	if (it != preserve_attachments_.end()) {
		preserve_attachments_.erase(it);
	}
}

void SubpassLayout::validate() const {
	/**
	 * TODO:
	 * - pipelineBindPoint must be VK_PIPELINE_BIND_POINT_GRAPHICS
	 * - colorAttachmentCount must be less than or equal to VkPhysicalDeviceLimits::maxColorAttachments
	 * - If the first use of an attachment in this render pass is as an input attachment, and the attachment is not also used as a color or depth / stencil attachment in the same subpass, then loadOp must not be VK_ATTACHMENT_LOAD_OP_CLEAR
	 * - If pResolveAttachments is not NULL, for each resolve attachment that does not have the value VK_ATTACHMENT_UNUSED, the corresponding color attachment must not have the value VK_ATTACHMENT_UNUSED
	 * - If pResolveAttachments is not NULL, the sample count of each element of pColorAttachments must be anything other than VK_SAMPLE_COUNT_1_BIT
	 * - Each element of pResolveAttachments must have a sample count of VK_SAMPLE_COUNT_1_BIT
	 * - Each element of pResolveAttachments must have the same VkFormat as its corresponding color attachment
	 * - All attachments in pColorAttachments that are not VK_ATTACHMENT_UNUSED must have the same sample count
	 * - If pDepthStencilAttachment is not VK_ATTACHMENT_UNUSED and any attachments in pColorAttachments are not VK_ATTACHMENT_UNUSED, they must have the same sample count
	 * - If any input attachments are VK_ATTACHMENT_UNUSED, then any pipelines bound during the subpass must not access those input attachments from the fragment shader
	 * - The attachment member of each element of pPreserveAttachments must not be VK_ATTACHMENT_UNUSED
	 * - Each element of pPreserveAttachments must not also be an element of any other member of the subpass description
	 * - If any attachment is used as both an input attachment and a color or depth / stencil attachment, then each use must use the same layout
	*/
}

const vk::SubpassDescription& SubpassLayout::getVkSubpassDescription() {
	// Check if the vk::SubpassDescription was already built.
	if (vk_subpass_description_.has_value()) {
		return vk_subpass_description_.value();
	}

	// Validate configuration. TODO: Make this optional?
	validate();

	// Build subpass description.
	vk_subpass_description_.emplace(vk::SubpassDescriptionFlags(), pipeline_bind_point_,
		input_attachments_.size(), (!input_attachments_.empty()) ? &input_attachments_[0] : nullptr,
		color_attachments_.size(), (!color_attachments_.empty()) ? &color_attachments_[0] : nullptr,
		(!resolve_attachments_.empty()) ? &resolve_attachments_[0] : nullptr,
		(depth_stencil_attachment_.has_value()) ? &depth_stencil_attachment_.value() : nullptr,
		preserve_attachments_.size(), (!preserve_attachments_.empty()) ? &preserve_attachments_[0] : nullptr);

	return vk_subpass_description_.value();
}

void SubpassLayout::addOrUpdateAttachment(std::vector<vk::AttachmentReference>& attachments, attachment_index_t index, vk::ImageLayout layout) {
	// Try to find existing attachment reference.
	auto it = std::find_if(attachments.begin(), attachments.end(), [index](const vk::AttachmentReference& attachment_ref) {
		return attachment_ref.attachment == index;

	});

	if (it != attachments.end()) {
		// Update existing attachment reference layout.
		if (it->layout != layout) {
			it->setLayout(layout);
			vk_subpass_description_.reset();
		}
	}
	else {
		// Add new attachment reference.
		attachments.emplace_back(index, layout);
		vk_subpass_description_.reset();
	}
}

void SubpassLayout::removeAttachment(std::vector<vk::AttachmentReference>& attachments, attachment_index_t index) {
	// Try to find attachment reference with the given attachment index.
	auto it = std::find_if(attachments.begin(), attachments.end(), [index](const vk::AttachmentReference& attachment_ref) {
		return attachment_ref.attachment == index;
	});


	// Remove if found.
	if (it != attachments.end()) {
		attachments.erase(it);
		vk_subpass_description_.reset();
	}
}

} ///! namespace vkr
