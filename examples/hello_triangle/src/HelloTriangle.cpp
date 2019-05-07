#include "HelloTriangle.h"

void HelloTriangle::createRenderPass() {
	logi::RenderPassLayout rp_layout(1u, 1u);
	vk::AttachmentDescription* attachment_desc = rp_layout.getAttachmentDescription(0u);

	attachment_desc->format = swap_chain.getColorFormat();
	attachment_desc->samples = vk::SampleCountFlagBits::e1;
	attachment_desc->loadOp = vk::AttachmentLoadOp::eClear;
	attachment_desc->storeOp = vk::AttachmentStoreOp::eStore;
	attachment_desc->stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	attachment_desc->stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	attachment_desc->initialLayout = vk::ImageLayout::eUndefined;
	attachment_desc->finalLayout = vk::ImageLayout::ePresentSrcKHR;

	logi::SubpassLayout* subpass = rp_layout.getSubpassLayout(0u);
	subpass->addColorAttachment(0u, vk::ImageLayout::eColorAttachmentOptimal);

	rp_layout.addDependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
		{}, vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite, {});

	render_pass = gpu.getProgramManager().createRenderPass(rp_layout);
}

void HelloTriangle::createGraphicalPipeline() {
	const logi::ProgramManager program_manager = gpu.getProgramManager();
	logi::ShaderModule vertex_shader = program_manager.loadShaderModule("./shaders/triangle.vert.spv");
	logi::ShaderModule fragment_shader = program_manager.loadShaderModule("./shaders/triangle.frag.spv");
	logi::PipelineLayout pipeline_layout = program_manager.createPipelineLayout({ logi::PipelineShaderStage(vk::ShaderStageFlagBits::eVertex, vertex_shader), logi::PipelineShaderStage(vk::ShaderStageFlagBits::eFragment, fragment_shader) });

	logi::PipelineState state;

	// INPUT ASSEMBLY STATE
	state.input_assembly.primitive_restart_enable = false;
	state.input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

	// VIEWPORT STATE
	vk::Viewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swap_chain.getExtent().width);
	viewport.height = static_cast<float>(swap_chain.getExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vk::Rect2D scissor{};
	scissor.offset = vk::Offset2D(0, 0);
	scissor.extent = swap_chain.getExtent();

	state.viewport.viewports.push_back(viewport);
	state.viewport.scissors.push_back(scissor);

	// RASTERIZER STATE
	state.rasterization.depth_clamp_enable = false;
	state.rasterization.rasterizer_discard_enable = false;
	state.rasterization.polygon_mode = vk::PolygonMode::eFill;
	state.rasterization.line_width = 1.0f;
	state.rasterization.cull_mode = vk::CullModeFlagBits::eBack;
	state.rasterization.front_face = vk::FrontFace::eClockwise;
	state.rasterization.depth_bias_enable = false;

	// MULTISAMPLING
	state.multisample.enable_sample_shading = false;
	state.multisample.rasterization_samples = vk::SampleCountFlagBits::e1;
	state.multisample.sample_mask = vk::SampleMask(0xFFFFFF);

	// COLOR BLEND STATE
	state.color_blend.enable_logic_op = false;
	state.color_blend.logic_op = vk::LogicOp::eCopy;

	vk::PipelineColorBlendAttachmentState color_blend_att{};
	color_blend_att.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	color_blend_att.blendEnable = false;
	state.color_blend.attachment_states.emplace_back(color_blend_att);
	state.color_blend.blend_constants = { 0, 0, 0, 0 };

	std::vector<logi::GraphicalPipelineCreateInfo> create_info = { logi::GraphicalPipelineCreateInfo(render_pass, 0, pipeline_layout, state) };
	graphical_pipeline = program_manager.createGraphicalPipelines(create_info)[0];
}

void HelloTriangle::createFrameBuffers() {
	for (const logi::ImageView& image_view : swap_chain.getImageViews()) {
		framebuffers.emplace_back(render_pass.createFramebuffer({ image_view }, swap_chain.getExtent().width, swap_chain.getExtent().height, 1));
	}
}

void HelloTriangle::recordCommandBuffers() {
	// Create graphical command buffers.
	for (size_t i = 0; i < cmd_buffers.size(); i++) {
		const logi::PrimaryCommandBuffer& cmd_buffer = cmd_buffers[i];
		cmd_buffer.begin(logi::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

		const std::vector<vk::ClearValue> clear_values = { vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f })) };
		const logi::RenderPassBeginInfo rp_begin_info(render_pass, framebuffers[i], vk::Rect2D(vk::Offset2D(), swap_chain.getExtent()), clear_values);

		cmd_buffer.beginRenderPass(rp_begin_info);
		cmd_buffer.bindPipeline(graphical_pipeline);
		cmd_buffer.draw(3);
		cmd_buffer.endRenderPass();
		cmd_buffer.end();
	}
}

void HelloTriangle::initialize() {
	createRenderPass();
	createGraphicalPipeline();
	createFrameBuffers();
	recordCommandBuffers();
}


void HelloTriangle::draw() {
	
}
