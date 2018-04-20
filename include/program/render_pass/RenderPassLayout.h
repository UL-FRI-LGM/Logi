#ifndef PROGRAM_RENDER_PASS_RENDER_PASS_LAYOUT_H
#define PROGRAM_RENDER_PASS_RENDER_PASS_LAYOUT_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <map>
#include <tuple>
#include <limits>
#include "base/Types.h"
#include "program/render_pass/SubpassLayout.h"

namespace vkr {

/**   
 * @brief	TODO:
 */
class RenderPassLayout {
public:
	/**
	 * @brief	Add an AttachmentDescription to the RenderPassLayout.
	 *
	 * @param	format				Specifies the format of the image that will be used for the attachment.
	 * @param	samples				Specifies sample counts supported for an image used for storage operations
	 * @param	load_op				Specifies how the contents of color and depth components of the attachment are treated at the beginning of the subpass.
	 * @param	store_op			Specifies how the contents of color and depth components of the attachment are treated at the end of the subpass.
	 * @param	stencil_load_op 	Specifies how the contents of stencil components of the attachment are treated at the beginning of the subpass.
	 * @param	stencil_store_op	Specifies how the contents of stencil components of the attachment are treated at the end of the last subpass.
	 * @param	initial_layout  	Layout the attachment image sub-resource will be in when a render pass instance begins.
	 * @param	final_layout		Layout the attachment image sub-resource will be transitioned to when a render pass instance ends.
	 * @param	flags				Used to specify additional properties of an attachment
	 *
	 * @return	AttachmentDescription index.
	 */
	attachment_index_t addAttachmentDescription(vk::Format format, vk::SampleCountFlagBits samples,
		vk::AttachmentLoadOp load_op, vk::AttachmentStoreOp store_op, vk::AttachmentLoadOp stencil_load_op, 
		vk::AttachmentStoreOp stencil_store_op, vk::ImageLayout initial_layout, vk::ImageLayout final_layout, const vk::AttachmentDescriptionFlags& flags);

	/**
	 * @brief	Remove the last added AttachmentDescription.
	 */
	void popAttachmentDescription();

	/**
	 * @brief	Retrievs attachment description with the given index.
	 *
	 * @param	index	Attachment description index.
	 *
	 * @return	Const reference to AttachmentDescription. Adding new or poping attachment description invalidates the reference.
	 */
	const vk::AttachmentDescription& getAttachmentDescription(attachment_index_t index) const;

	/**
	 * @brief	Retrieve number of attachments.
	 *
	 * @return	Attachment count.
	 */
	attachment_index_t attachmentDescriptionCount() const;

	/**
	 * @brief	Add a new SubpassLayout to the RenderPassLayout.
	 *
	 * @return	Tuple containing index of the created SubpassLayout and a pointer to it.
	 */
	std::tuple<subpass_index_t, SubpassLayout*> addSubpassLayout();

	/**
	 * @brief	Retrieve a SubpassLayout with the given index.
	 *
	 * @param	index	SubpassLayout index.
	 *
	 * @return	Pointer to the requested SubpassLayout.
	 */
	SubpassLayout* getSubpass(subpass_index_t index);

	/**
	 * @brief	Removes last added SubpassLayout and all its dependencies.
	 */
	void popSubpass();

	/**
	 * @brief	Introduce a dependency between the given source and destination subpass. 
	 * 			https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#synchronization-pipeline-stages-masks
	 *
	 * @param	src_subpass			Pointer to source SubpassLayout.
	 * @param	dst_subpass			Pointer to destination SubpassLayout.
	 * @param	src_stage_mask  	Source stage mask.
	 * @param	dst_stage_mask  	Destination stage mask.
	 * @param	src_access_mask 	Source access mask.
	 * @param	dst_access_mask 	Destination access mask.
	 * @param	dependency_flags	Bitmask specifying how execution and memory dependencies are formed.
	 *
	 * @return	Id of the subpass dependency.
	 */
	dependency_id_t addSubpassDependency(const SubpassLayout* src_subpass, const SubpassLayout* dst_subpass, const vk::PipelineStageFlags& src_stage_mask, const vk::PipelineStageFlags& dst_stage_mask,
		const vk::AccessFlags& src_access_mask, const vk::AccessFlags& dst_access_mask, const vk::DependencyFlags& dependency_flags);
	
	/**
	 * @brief	Introduce a dependency between the given source and destination subpass.
	 * 			https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#synchronization-pipeline-stages-masks
	 *
	 * @param	src_subpass_index	Index of the source SubpassLayout.
	 * @param	dst_subpass_index	Index of the destination SubpassLayout.
	 * @param	src_stage_mask  	Source stage mask.
	 * @param	dst_stage_mask  	Destination stage mask.
	 * @param	src_access_mask 	Source access mask.
	 * @param	dst_access_mask 	Destination access mask.
	 * @param	dependency_flags	Bitmask specifying how execution and memory dependencies are formed.
	 *
	 * @return	Id of the subpass dependency.
	 */
	dependency_id_t addSubpassDependency(subpass_index_t src_subpass_index, subpass_index_t dst_subpass_index, const vk::PipelineStageFlags& src_stage_mask, const vk::PipelineStageFlags& dst_stage_mask,
		const vk::AccessFlags& src_access_mask, const vk::AccessFlags& dst_access_mask, const vk::DependencyFlags& dependency_flags);

	/**
	 * @brief	Retrieve SubpassDependency with the given id.
	 *
	 * @param	id	SubpassDependency id.
	 *
	 * @return	Const reference to the requested SubpassDependency.
	 */
	const vk::SubpassDependency& getSubpassDependency(dependency_id_t id) const;

	/**
	 * @brief	Removes the SubpassDependency with the given id.
	 *
	 * @param	id	SubpassDependency identifier.
	 */
	void removeSubpassDependency(dependency_id_t id);

	/**
	 * @brief TODO:
	 */
	void validate() const;


	vk::RenderPass buildRenderPass(vk::Device device) const;

private:
	std::vector<std::unique_ptr<vk::AttachmentDescription>> attachment_descriptions_;	///< Render pass attachment descriptions.
	std::vector<std::unique_ptr<SubpassLayout>> subpasses_;								///< Render pass subpasses.
	std::map<dependency_id_t, vk::SubpassDependency> subpass_dependencies_;				///< Render pass dependencies.
};

};

#endif ///!	HEADER_GUARD