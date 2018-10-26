#include "base/VulkanInstance.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "base/SwapChain.h"
#include "memory/Framebuffer.h"
#include "base/ExtensionObject.h"

class HelloTriangle {
public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
		std::cerr << "validation layer: " << msg << std::endl;

		return VK_FALSE;
	};

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return extensions;
	}

	void initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	}

	void createInstance() {
		logi::ApplicationInfo app_info("test", 1, "test", 1);
		logi::InstanceConfig instance_config{};
		instance_config.extensions = getRequiredExtensions();
		instance_config.extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		instance_config.validation_layers = { "VK_LAYER_LUNARG_standard_validation" };
		instance_config.enable_validation = true;

		instance = logi::VulkanInstance(app_info, instance_config);

		instance.setupDebugCallback(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning , debugCallback);
	}

	void initSurface() {
		VkSurfaceKHR vk_surface;

		if (glfwCreateWindowSurface(static_cast<VkInstance>(instance.getVkHandle()), window, nullptr, &vk_surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}

		surface = vk::SurfaceKHR(vk_surface);
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
		
		for (auto it = devices.begin(); it != devices.end(); ++it) {
			if (it->properties().deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
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

		gpu = physical_device.createLogicalDevice(configurations, { VK_KHR_SWAPCHAIN_EXTENSION_NAME });
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

		swapchain.create(swapchain_extent.width, swapchain_extent.height);
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
		logi::Shader vertex_shader = program_manager.loadShader("./shaders/triangle.vert.spv", vk::ShaderStageFlagBits::eVertex);
		logi::Shader fragment_shader = program_manager.loadShader("./shaders/triangle.frag.spv", vk::ShaderStageFlagBits::eFragment);
		logi::PipelineLayout pipeline_layout = program_manager.createPipelineLayout({ vertex_shader, fragment_shader });

		logi::PipelineState state;

		// INPUT ASSEMBLY STATE
		state.input_assembly.primitive_restart_enable = false;
		state.input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

		// VIEWPORT STATE
		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = swapchain_extent.width;
		viewport.height = swapchain_extent.height;
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

	void createFramebuffers() {
		for (const logi::ImageView& image_view : swapchain.getImageViews()) {
			framebuffers.emplace_back(render_pass.createFramebuffer({ image_view }, swapchain_extent.width, swapchain_extent.height, 1));
		}
	}

	void buildCommandBuffers() {
		for (const logi::Framebuffer& framebuffer : framebuffers) {
			cmd_buffers.emplace_back(command_pool.createPrimaryCommandBuffer());
			logi::PrimaryCommandBuffer& cmd_buffer = cmd_buffers.back();
			const vk::CommandBuffer& vk_cmd_buffer = cmd_buffers.back().getVkHandle();

			cmd_buffer.begin(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

			const std::vector<vk::ClearValue> clear_values = { vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f })) };
			cmd_buffer.beginRenderPass(render_pass, framebuffer, vk::Rect2D(vk::Offset2D(), swapchain_extent), clear_values);
			cmd_buffer.bindPipeline(graphic_pipeline);
			cmd_buffer.draw(3);
			cmd_buffer.endRenderPass();
			cmd_buffer.end();
		}	
	}

	void buildSyncObjects() {
		for (size_t i = 0; i < max_frames_in_flight; i++) {
			image_available_semaphores.emplace_back(gpu.createSemaphore());
			render_finished_semaphores.emplace_back(gpu.createSemaphore());
			in_flight_fences.emplace_back(gpu.createFence(vk::FenceCreateFlagBits::eSignaled));
		}
	}

	void drawFrame() {
		in_flight_fences[current_frame].wait(std::numeric_limits<uint64_t>::max());
		in_flight_fences[current_frame].reset();

		uint32_t image_index = swapchain.acquireNextImage(image_available_semaphores[current_frame]).value;

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
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			drawFrame();
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
		createFramebuffers();
		buildCommandBuffers();
		buildSyncObjects();
		mainLoop();
	}

private:
	GLFWwindow* window;
	vk::SurfaceKHR surface;

	static const size_t max_frames_in_flight = 2;
	static const uint32_t width = 1920u;
	static const uint32_t height = 1080;

	logi::VulkanInstance instance{};
	logi::PhysicalDevice physical_device{};
	logi::LogicalDevice gpu{};
	logi::ProgramManager program_manager{};
	logi::AllocationManager allocation_manager{};
	logi::CommandPool command_pool{};

	logi::GraphicalPipeline graphic_pipeline{};
	logi::RenderPass render_pass{};

	uint32_t render_family_index;
	logi::Queue render_queue{};
	uint32_t present_family_index;
	logi::Queue present_queue{};

	logi::SwapChain swapchain{};
	vk::Extent2D swapchain_extent{ width, height };

	std::vector<logi::Framebuffer> framebuffers{};
	std::vector<logi::PrimaryCommandBuffer> cmd_buffers{};

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
