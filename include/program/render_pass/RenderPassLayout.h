#ifndef PROGRAM_RENDER_PASS_RENDER_PASS_LAYOUT_H
#define PROGRAM_RENDER_PASS_RENDER_PASS_LAYOUT_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "base/Types.h"
#include "program/render_pass/SubpassLayout.h"

namespace logi {

/**
 *	@brief Class used to build RenderPass configuration.
 */
class RenderPassLayout {
public:
	/**
	 * @brief	Create new RenderPassLayout with attachment_count attachments and subpass_count sub passes.
	 * 
	 * @param	attachment_count	Number of attachments. 
	 * @param	subpass_count		Number of sub passes.
	 */
	RenderPassLayout(size_t attachment_count, size_t subpass_count);

	/**
	 * @brief	Set the configuration for the attachment with the given index.
	 * 
	 * @param	index			Attachment index.
	 * @param	attachment_desc	Attachment description.
	 */
	void setAttachmentDescription(attachment_index_t index, const vk::AttachmentDescription& attachment_desc);

	/**
	 * @brief	Retrieve pointer to the description of the attachment with the given index.
	 * 
	 * @param	index	Attachment index.
	 * @return	Pointer to the attachment description,
	 */
	vk::AttachmentDescription* getAttachmentDescription(attachment_index_t index);

	/**
	 * @brief	Set sub pass layout for the sub pass with the given index.
	 * 
	 * @param	index			Sub pass index.
	 * @param	subpass_layout	Sub pass layout.
	 */
	void setSubpassLayout(subpass_index_t index, const SubpassLayout& subpass_layout);

	/**
	 * @brief	Retrieve pointer to the layout of the sub pass with the given index.
	 * 
	 * @param	index	Sub pass index.
	 * @return	Pointer to the sub pass layout.
	 */
	SubpassLayout* getSubpassLayout(subpass_index_t index);

	/**
	 * @brief	Create dependency between two sub passes or between a sub pass and external synchronization point.
	 * 
	 * @param	src					Index of the source sub pass [0, subpass_count) or VK_SUBPASS_EXTERNAL if external synchronization point.
	 * @param	dst					Index of the destination sub pass [0, subpass_count) or VK_SUBPASS_EXTERNAL if external synchronization point.
	 * @param	src_stage_mask  	Source stage mask.
	 * @param	dst_stage_mask  	Destination stage mask.
	 * @param	src_access_mask 	Source access mask.
	 * @param	dst_access_mask 	Destination access mask.
	 * @param	dependency_flags	Bitmask specifying how execution and memory dependencies are formed.
	 */
	void addDependency(subpass_index_t src, subpass_index_t dst, vk::PipelineStageFlags src_stage_mask, vk::PipelineStageFlags dst_stage_mask, vk::AccessFlags src_access_mask, vk::AccessFlags dst_access_mask, vk::DependencyFlags dependency_flags);

	/**
	 * @brief	Retrieve pointer to the dependency between source and destination sub pass.
	 *
	 * @param	src	Index of the source sub pass [0, subpass_count) or VK_SUBPASS_EXTERNAL if external synchronization point.
	 * @param	dst Index of the destination sub pass [0, subpass_count) or VK_SUBPASS_EXTERNAL if external synchronization point.
	 * @return	Pointer to the sub pass dependency.
	 */
	vk::SubpassDependency* getDependency(subpass_index_t src, subpass_index_t dst);

	/**
	 * @brief	Remove dependency between source and destination sub passes.
	 * 
	 * @param	src	Index of the source sub pass [0, subpass_count) or VK_SUBPASS_EXTERNAL if external synchronization point.
	 * @param	dst Index of the destination sub pass [0, subpass_count) or VK_SUBPASS_EXTERNAL if external synchronization point.
	 */
	void removeDependency(subpass_index_t src, subpass_index_t dst);

	/**
	 * @brief	Build create info from the aggregated configuration.
	 * @note	Modifying or copying RenderPassLayout object makes generated structure invalid.
	 *
	 * @return	Pointer to the built render pass create info structure.
	 */
	vk::RenderPassCreateInfo* buildCreateInfo();

private:
	std::vector<vk::AttachmentDescription> attachments_;	/// Vector of attachment descriptions.
	std::vector<SubpassLayout> subpasses_;					/// Vector of sub pass layouts.
	std::vector<vk::SubpassDependency> dependencies_;		/// Vector of sub pass dependencies.

	// TODO(Primoz): Add extension structures.

	std::vector<vk::SubpassDescription> vk_subpasses_;		/// Vector of built sub pass descriptions.
	vk::RenderPassCreateInfo vk_create_info_;
};


}

#endif ///!	HEADER_GUARD