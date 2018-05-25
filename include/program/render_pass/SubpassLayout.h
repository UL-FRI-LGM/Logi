#ifndef PROGRAM_RENDER_PASS_SUBPASS_H
#define PROGRAM_RENDER_PASS_SUBPASS_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <optional>
#include <functional>
#include "base/Types.h"

namespace vkr {

/** @brief	Provides interface to accumulate and validate the vk::SubpassDescription data. */
class SubpassLayout {
public:
	/**
	 * @brief	Creates new empty SubpassLayout object.
	 */
	SubpassLayout();

	/**
	 * @brief	Adds attachment with the given index to the list of input attachments for this subpass and specifies its layout.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 * @param	layout	Attachment layout in which the attachment should be during this subpass.
	 */
	void addInputAttachment(const attachment_index_t index, const vk::ImageLayout layout);

	/**
	 * @brief	Removes attachment with the given index from the list of input attachments.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 */
	void removeInputAttachment(const attachment_index_t index);

	/** 
	 * @brief	Clears the input attachments list.
	 */
	void clearInputAttachments();

	/**
	 * @brief	Adds attachment with the given index to the list of color attachments for this subpass and specifies its layout.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 * @param	layout	Attachment layout in which the attachment should be during this subpass.
	 */
	void addColorAttachment(const attachment_index_t index, const vk::ImageLayout layout);

	/**
	 * @brief	Removes attachment with the given index from the list of color attachments.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 */
	void removeColorAttachment(const attachment_index_t index);

	/**
	 * @brief	Clears the color attachments list.
	 */
	void clearColorAttachments();

	/**
	 * @brief	Adds attachment with the given index to the list of resolve attachments for this subpass and specifies its layout.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 * @param	layout	Attachment layout in which the attachment should be during this subpass.
	 */
	void addResolveAttachment(const attachment_index_t index, const vk::ImageLayout layout);

	/**
	 * @brief	Removes attachment with the given index from the list of resolve attachments.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 */
	void removeResolveAttachment(const attachment_index_t index);

	/**
	 * @brief	Clears the resolve attachments list.
	 */
	void clearResolveAttachments();

	/**
	 * @brief	Specifies which attachment will be used for depth stencil data and specifies its layout.
	 *
	 * @param	index	Attachment index (references RenderPassLayout attachments).
	 * @param	layout	Attachment layout in which the attachment should be during this subpass.
	 */
	void setDepthStencilAttachment(const attachment_index_t index, const vk::ImageLayout layout);

	/** 
	 * @brief	Clears current depth stencil attachment reference.
	 */
	void unsetDepthStencilAttachment();

	/**
	 * @brief	Stores indices of the attachments that will not be used but should be preserved.
	 *
	 * @param	index		Attachment index (references RenderPassLayout attachments).
	 * @param	indices		Additional attachment indices.	
	 */
	void preserveAttachments(const attachment_index_t index, const attachment_index_t indices ...);

	/**
	 * @brief	Remove attachment indices from preserve attachment list.
	 *
	 * @param	index		Attachment index (references RenderPassLayout attachments).
	 * @param	indices		Additional attachment indices.
	 */
	void dontPreserveAttachments(const attachment_index_t index, const attachment_index_t indices ...);

	/**
	 * @brief	Builds vk::SubpassDescription or retrieves existing one if already built.
	 * 			
	 * @return	vk::SubpassDescription containing subpass configuration.
	 */
	vk::SubpassDescription createVkSubpassDescription() const;

protected:
	/**
	 * @brief	Helper function used to update or add additional vk::AttachmentReference to the attachment reference list.
	 *
	 * @param	attachments			Reference to attachment reference list.
	 * @param 	attachment_index	Attachment index (references RenderPassLayout attachments).
	 * @param 	layout				Attachment layout in which the attachment should be during this subpass.
	 */
	static void addOrUpdateAttachment(std::vector<vk::AttachmentReference>& attachments, const attachment_index_t index, const vk::ImageLayout layout);

	/**
	 * @brief	Helper function used to remove the attachment with the given index from the attachment reference list
	 *
	 * @param	attachments			Reference to attachment reference list.
	 * @param 	attachment_index	Attachment index (references RenderPassLayout attachments).
	 */
	static void removeAttachment(std::vector<vk::AttachmentReference>& attachments, const attachment_index_t index);

private:
	vk::PipelineBindPoint					pipeline_bind_point_;		///< Specifies whether this is a compute or graphics subpass.
	std::vector<vk::AttachmentReference>	input_attachments_;			///< Lists which of the render pass’s attachments can be read in the fragment shader stage during the subpass.
	std::vector<vk::AttachmentReference>	color_attachments_;			///< Lists which of the render pass’s attachments will be used as color attachments in the subpass.
	std::vector<vk::AttachmentReference>	resolve_attachments_;		///< Lists which of the render pass’s attachments are resolved to at the end of the subpass.
	std::optional<vk::AttachmentReference>	depth_stencil_attachment_;  ///< Specifying which attachment will be used for depth/stencil data subpass.
	std::vector<attachment_index_t>			preserve_attachments_;		///< Lists which of the render pass’s attachments will not be used by a subpass, but whose contents must be preserved.
};

};

#endif ///!	HEADER_GUARD