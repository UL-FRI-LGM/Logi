#include "logi/base/VulkanInstance.h"
#define GLFW_INCLUDE_VULKAN
#include <base/GLFWManager.h>
#include <glm/glm.hpp>
#include "logi/base/SwapChain.h"
#include "logi/memory/Framebuffer.h"
#include <algorithm>
#include <iostream>
#include "logi/base/ExtensionObject.h"
#include "logi/commands/PrimaryCommandBuffer.h"

struct Pixel {
	float r, g, b, a;
};

struct PathtracerInput {
	glm::vec2 mouse_pos;
	float timestamp;
	bool redraw;
};

struct GraphicInput {
	float frame_count;
};

class HelloTriangle {
public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
		std::cout << "validation layer: " << msg << std::endl;

		return VK_FALSE;
	};

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return extensions;
	}

	void initWindow() {
		window = GLFWManager::instance().createWindow("Example", width, height);
	}

	void createInstance() {
		logi::InstanceConfig instance_config{};
		instance_config.extensions = getRequiredExtensions();
		instance_config.extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		instance_config.validation_layers = { "VK_LAYER_LUNARG_standard_validation", "VK_LAYER_RENDERDOC_Capture" };
    
		instance = logi::VulkanInstance(instance_config);

		instance.setupDebugCallback(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning, debugCallback);
	}

	void initSurface() {
		surface = window.createWindowSurface(instance.getVkHandle()).value;
	}

	void selectDevice() {
		// Select GPU
		const std::vector<logi::PhysicalDevice>& devices = instance.devices();

		for (auto it = devices.begin(); it != devices.end(); ++it) {
			if (it->properties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
				physical_device = *it;
				return;
			}
		}

		assert(false);
	}

	void initializeDevice() {
		// Configure queue families and initialize device.
		std::vector<logi::QueueFamilyProperties> family_properties = physical_device.queueFamilyProperties();

		present_family_index = std::numeric_limits<uint32_t>::max();
		render_family_index = std::numeric_limits<uint32_t>::max();
		std::vector<logi::QueueFamilyConfig> configurations;

		// Search for graphical queue family.
		for (const logi::QueueFamilyProperties& properties : family_properties) {
			if (properties.queue_flags | vk::QueueFlagBits::eGraphics) {
				configurations.emplace_back(logi::QueueFamilyConfig(properties, 1u));
				render_family_index = properties.family_index;
				break;
			}
		}

		// Try to find graphical queue.
		if (configurations.empty()) {
			throw std::runtime_error("Failed to find graphical queue.");
		}

		// If graphical queue supports surface present use it for presenting, otherwise try to find present queue.
		if (physical_device.supportsSurfacePresent(surface, configurations[0].properties)) {
			present_family_index = render_family_index;
		}
		else {
			for (const logi::QueueFamilyProperties& properties : family_properties) {
				if (physical_device.supportsSurfacePresent(surface, properties)) {
					configurations.emplace_back(logi::QueueFamilyConfig(properties, 1u));
					present_family_index = properties.family_index;
					break;
				}
			}

			if (present_family_index == std::numeric_limits<uint32_t>::max()) {
				throw std::runtime_error("Failed to find queue family that supports presentation.");
			}
		}

		gpu = physical_device.createLogicalDevice(logi::LogicalDeviceConfig(configurations, std::vector<const char*>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }));
	  program_manager = gpu.getProgramManager();
		allocation_manager = gpu.getAllocationManager();
		command_pool = gpu.getQueueFamily(render_family_index).createCommandPool();
		render_queue = gpu.getQueueFamily(render_family_index).getQueue(0u);
		present_queue = gpu.getQueueFamily(present_family_index).getQueue(0u);
	}

	void initializeSwapchain() {
		if (render_family_index == present_family_index) {
			swapchain = gpu.createSwapchain(surface, present_family_index);
		}
		else {
			swapchain = gpu.createSwapchain(surface, present_family_index, { render_family_index, present_family_index });
		}

		swapchain.create(swapchain_extent, false);
	}

	void createRenderPass() {
		logi::RenderPassLayout rp_layout(1u, 1u);
		vk::AttachmentDescription* attachment_desc = rp_layout.getAttachmentDescription(0u);

		attachment_desc->format = swapchain.getColorFormat();
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


		render_pass = program_manager.createRenderPass(rp_layout);
	}

	void createGraphicPipeline() {
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
		viewport.width = static_cast<float>(swapchain_extent.width);
		viewport.height = static_cast<float>(swapchain_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D(0, 0);
		scissor.extent = swapchain_extent;

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
		graphic_pipeline = program_manager.createGraphicalPipelines(create_info)[0];
	}

	void createComputePipeline() {
		const logi::ShaderModule compute_shader = program_manager.loadShaderModule("./shaders/pathtrace.comp.spv");
		const logi::PipelineLayout pipeline_layout = program_manager.createPipelineLayout({ logi::PipelineShaderStage(vk::ShaderStageFlagBits::eCompute, compute_shader) });

		compute_pipeline = program_manager.createComputePipelines({ logi::ComputePipelineCreateInfo(pipeline_layout) })[0];
	}

	void initBuffersAndDescriptorSets() {

		// Initialize texture.
		texture.image = allocation_manager.createImage(logi::ImageConfiguration(vk::ImageType::e2D, vk::Format::eR32G32B32A32Sfloat, vk::Extent3D(width, height, 1), 1, 1,
			vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eStorage, logi::MemoryUsage::GPU_ONLY, vk::ImageLayout::eUndefined));

		logi::SamplerConfiguration sampler_config(vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eClampToBorder, vk::SamplerAddressMode::eClampToBorder,
			vk::SamplerAddressMode::eClampToBorder, 0.0f, false, 1.0f, false, vk::CompareOp::eNever, 0.0f, 0.0f, vk::BorderColor::eFloatOpaqueWhite, false);

		texture.sampler = gpu.createSampler(sampler_config);

		logi::ImageViewConfiguration image_view_config(vk::ImageViewType::e2D, vk::Format::eR32G32B32A32Sfloat, vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA), vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

		texture.image_view = texture.image.createImageView(image_view_config);

		graphic_input_buffer = allocation_manager.createBuffer(logi::BufferConfiguration(sizeof(GraphicInput), vk::BufferUsageFlagBits::eUniformBuffer, logi::MemoryUsage::CPU_TO_GPU));
		compute_input_buffer = allocation_manager.createBuffer(logi::BufferConfiguration(sizeof(PathtracerInput), vk::BufferUsageFlagBits::eUniformBuffer, logi::MemoryUsage::CPU_TO_GPU));

		descriptor_pool = gpu.createDescriptorPool(graphic_pipeline.layout().getDescriptorsCount() + compute_pipeline.layout().getDescriptorsCount());

		compute_desc_set = descriptor_pool.allocateDescriptorSet(compute_pipeline.layout().getDescriptorSetLayout(0));
		graphic_desc_set = descriptor_pool.allocateDescriptorSet(graphic_pipeline.layout().getDescriptorSetLayout(0));

		logi::DescriptorSetUpdate desc_update = gpu.createDescriptorUpdate();
		desc_update.write(logi::DescriptorImageWrite(compute_desc_set, 0, 0, { logi::DescriptorImageInfo(texture.sampler, texture.image_view, vk::ImageLayout::eGeneral)}));
		desc_update.write(logi::DescriptorBufferWrite(compute_desc_set, 1, 0, { logi::DescriptorBufferInfo(compute_input_buffer, 0, sizeof(PathtracerInput)) }));
		desc_update.write(logi::DescriptorImageWrite(graphic_desc_set, 0, 0, { logi::DescriptorImageInfo(texture.sampler, texture.image_view, vk::ImageLayout::eGeneral) }));
		desc_update.write(logi::DescriptorBufferWrite(graphic_desc_set, 1, 0, { logi::DescriptorBufferInfo(graphic_input_buffer, 0, sizeof(GraphicInput)) }));
		
		desc_update.execute();
	}

	void createFramebuffers() {
		for (const logi::ImageView& image_view : swapchain.getImageViews()) {
			framebuffers.emplace_back(render_pass.createFramebuffer({ image_view }, swapchain_extent.width, swapchain_extent.height, 1));
		}
	}

	void buildCommandBuffers() {
		// Create compute command buffers.
		compute_cmd_buffer = command_pool.createPrimaryCommandBuffer();
		compute_cmd_buffer.begin();
		compute_cmd_buffer.bindPipeline(compute_pipeline);
		compute_cmd_buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, compute_pipeline.layout(), 0, { compute_desc_set });
		compute_cmd_buffer.dispatch(static_cast<uint32_t>(ceil(width / float(32))), static_cast<uint32_t>(ceil(height / float(32))), 1);
		compute_cmd_buffer.end();

		// Create graphical command buffers.
		for (const logi::Framebuffer& framebuffer : framebuffers) {
			cmd_buffers.emplace_back(command_pool.createPrimaryCommandBuffer());
			logi::PrimaryCommandBuffer& cmd_buffer = cmd_buffers.back();
			cmd_buffer.begin(logi::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

			const std::vector<vk::ClearValue> clear_values = { vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f })) };
		  const logi::RenderPassBeginInfo rp_begin_info(render_pass, framebuffer, vk::Rect2D(vk::Offset2D(), swapchain_extent), clear_values);

			cmd_buffer.beginRenderPass(rp_begin_info);
			cmd_buffer.bindPipeline(graphic_pipeline);
			cmd_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, graphic_pipeline.layout(), 0, { graphic_desc_set });
			cmd_buffer.draw(3);
			cmd_buffer.endRenderPass();
			cmd_buffer.end();
		}
	}

	void buildSyncObjects() {
		compute_fence = gpu.createFence();

		for (size_t i = 0; i < max_frames_in_flight; i++) {
			image_available_semaphores.emplace_back(gpu.createSemaphore());
			render_finished_semaphores.emplace_back(gpu.createSemaphore());
			in_flight_fences.emplace_back(gpu.createFence(vk::FenceCreateFlagBits::eSignaled));
		}
	}

	void drawFrame() {
		vk::SubmitInfo submit_info_compute;
		submit_info_compute.commandBufferCount = 1;
		submit_info_compute.pCommandBuffers = &compute_cmd_buffer.getVkHandle();

		render_queue.submit({ submit_info_compute }, compute_fence);
		compute_fence.wait(100000000000);
		compute_fence.reset();


		in_flight_fences[current_frame].wait(std::numeric_limits<uint64_t>::max());
		in_flight_fences[current_frame].reset();

	  const uint32_t image_index = swapchain.acquireNextImage(image_available_semaphores[current_frame]).value;

		static const vk::PipelineStageFlags wait_stages{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

		vk::SubmitInfo submit_info;
		submit_info.pWaitDstStageMask = &wait_stages;
		submit_info.pWaitSemaphores = &image_available_semaphores[current_frame].getVkHandle();
		submit_info.waitSemaphoreCount = 1u;

		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &cmd_buffers[image_index].getVkHandle();

		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = &render_finished_semaphores[current_frame].getVkHandle();

		render_queue.submit({ submit_info }, in_flight_fences[current_frame]);

		swapchain.queuePresent(present_queue, render_finished_semaphores[current_frame]);
    
		current_frame = (current_frame + 1) % max_frames_in_flight;
	}

	void mainLoop() {
		auto ref_ts = Time::now();
		graphic_input.frame_count = 1;

		while (!window.shouldClose()) {
			input_ubo.timestamp = std::chrono::duration<float, std::micro>(Time::now() - ref_ts).count() / 1000000;
			compute_input_buffer.writeData(&input_ubo, sizeof(PathtracerInput));
			graphic_input_buffer.writeData(&graphic_input, sizeof(GraphicInput));

			glfwPollEvents();
			drawFrame();

			graphic_input.frame_count++;

			if (glfwGetMouseButton(window.glfwHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				double x, y;
				glfwGetCursorPos(window.glfwHandle(), &x, &y);

				input_ubo.mouse_pos.x = std::min(std::max(0.0, x), (double)width) / width;
				input_ubo.mouse_pos.y = 1 - std::min(std::max(0.0, y), (double)height) / height;
				input_ubo.redraw = true;
				graphic_input.frame_count = 1;
			} else {
				input_ubo.redraw = false;
			}
		}

		gpu.waitIdle();
	}

	void start() {
		initWindow();
		createInstance();
		initSurface();
		selectDevice();
		initializeDevice();
		initializeSwapchain();

		createRenderPass();
		createGraphicPipeline();
		createComputePipeline();
		initBuffersAndDescriptorSets();
		createFramebuffers();
		buildCommandBuffers();

		buildSyncObjects();
		mainLoop();
	}

private:
	using Time = std::chrono::high_resolution_clock;
	
	struct Texture {
		logi::Image image{};
		logi::ImageView image_view{};
		logi::Sampler sampler{};
	};

	Window window;
	vk::SurfaceKHR surface;

	static const size_t max_frames_in_flight = 2;
	static const uint32_t width = 1920u;
	static const uint32_t height = 1080;

	logi::VulkanInstance instance{};
	logi::PhysicalDevice physical_device{};
	logi::LogicalDevice gpu{};
	logi::ProgramManager program_manager{};
	logi::AllocationManager allocation_manager{};
	logi::DescriptorPool descriptor_pool{};
	logi::CommandPool command_pool{};

	logi::GraphicalPipeline graphic_pipeline{};
	logi::RenderPass render_pass{};

	logi::ComputePipeline compute_pipeline{};

	Texture texture{};
	logi::Buffer graphic_input_buffer{};
	logi::Buffer compute_input_buffer{};
	logi::DescriptorSet compute_desc_set{};
	PathtracerInput input_ubo{};
	GraphicInput graphic_input{};

	logi::DescriptorSet graphic_desc_set{};

	uint32_t render_family_index;
	logi::Queue render_queue{};
	uint32_t present_family_index;
	logi::Queue present_queue{};

	logi::SwapChain swapchain{};
	vk::Extent2D swapchain_extent{ width, height };

	std::vector<logi::Framebuffer> framebuffers{};
	std::vector<logi::PrimaryCommandBuffer> cmd_buffers{};
	logi::PrimaryCommandBuffer compute_cmd_buffer{};

	logi::Fence compute_fence;
	std::vector<logi::Fence> in_flight_fences;
	std::vector<logi::Semaphore> image_available_semaphores;
	std::vector<logi::Semaphore> render_finished_semaphores;
	size_t current_frame = 0u;
};

int main() {

	{
		HelloTriangle example;
		example.start();
	}


	system("pause");
}
