#include "program/render_pass/SubpassLayout.h"

namespace vkr {

SubpassLayout::SubpassLayout()
	: pipeline_bind_point_(vk::PipelineBindPoint::eGraphics), input_attachments_(), color_attachments_(), resolve_attachments_(), depth_stencil_attachment_(), preserve_attachments_() {
}

void SubpassLayout::addInputAttachment(const attachment_index_t index, const vk::ImageLayout layout) {
	addOrUpdateAttachment(input_attachments_, index, layout);
}

void SubpassLayout::removeInputAttachment(const attachment_index_t index) {
	removeAttachment(input_attachments_, index);
}

void SubpassLayout::clearInputAttachments() {
	input_attachments_.clear();
}

void SubpassLayout::addColorAttachment(const attachment_index_t index, const vk::ImageLayout layout) {
	addOrUpdateAttachment(color_attachments_, index, layout);
}

void SubpassLayout::removeColorAttachment(const attachment_index_t index) {
	removeAttachment(color_attachments_, index);
}

void SubpassLayout::clearColorAttachments() {
	color_attachments_.clear();
}

void SubpassLayout::addResolveAttachment(const attachment_index_t index, const vk::ImageLayout layout) {
	addOrUpdateAttachment(resolve_attachments_, index, layout);
}

void SubpassLayout::removeResolveAttachment(const attachment_index_t index) {
	removeAttachment(resolve_attachments_, index);
}

void SubpassLayout::clearResolveAttachments() {
	resolve_attachments_.clear();
}

void SubpassLayout::setDepthStencilAttachment(const attachment_index_t index, const vk::ImageLayout layout) {
	depth_stencil_attachment_.emplace(index, layout);
}

void SubpassLayout::unsetDepthStencilAttachment() {
	if (depth_stencil_attachment_.has_value()) {
		depth_stencil_attachment_.reset();
	}
}

void SubpassLayout::preserveAttachments(const attachment_index_t index, const attachment_index_t indices ...) {
	// Check if the attachment is already among preserve attachments.
	const auto it = std::find(preserve_attachments_.begin(), preserve_attachments_.end(), index);

	// Add attachment index if not found.
	if (it != preserve_attachments_.end()) {
		preserve_attachments_.emplace_back(index);
	}
}

void SubpassLayout::dontPreserveAttachments(const attachment_index_t index, const attachment_index_t indices ...) {
	// Try to find preserved attachment index.
	const auto it = std::find(preserve_attachments_.begin(), preserve_attachments_.end(), index);

	// Remove if found.
	if (it != preserve_attachments_.end()) {
		preserve_attachments_.erase(it);
	}
}

 vk::SubpassDescription SubpassLayout::createVkSubpassDescription() const {
	// Build sub pass description.
	return vk::SubpassDescription(vk::SubpassDescriptionFlags(), pipeline_bind_point_,
		input_attachments_.size(), (!input_attachments_.empty()) ? &input_attachments_[0] : nullptr,
		color_attachments_.size(), (!color_attachments_.empty()) ? &color_attachments_[0] : nullptr,
		(!resolve_attachments_.empty()) ? &resolve_attachments_[0] : nullptr,
		(depth_stencil_attachment_.has_value()) ? &depth_stencil_attachment_.value() : nullptr,
		preserve_attachments_.size(), (!preserve_attachments_.empty()) ? &preserve_attachments_[0] : nullptr);
}

void SubpassLayout::addOrUpdateAttachment(std::vector<vk::AttachmentReference>& attachments, const attachment_index_t index, const vk::ImageLayout layout) {
	// Try to find existing attachment reference.
	auto it = std::find_if(attachments.begin(), attachments.end(), [index](const vk::AttachmentReference& attachment_ref) {
		return attachment_ref.attachment == index;

	});

	if (it != attachments.end()) {
		// Update existing attachment reference layout.
		if (it->layout != layout) {
			it->setLayout(layout);
		}
	}
	else {
		// Add new attachment reference.
		attachments.emplace_back(index, layout);
	}
}

void SubpassLayout::removeAttachment(std::vector<vk::AttachmentReference>& attachments, const attachment_index_t index) {
	// Try to find attachment reference with the given attachment index.
	auto it = std::find_if(attachments.begin(), attachments.end(), [index](const vk::AttachmentReference& attachment_ref) {
		return attachment_ref.attachment == index;
	});


	// Remove if found.
	if (it != attachments.end()) {
		attachments.erase(it);
	}
}

} ///! namespace vkr
