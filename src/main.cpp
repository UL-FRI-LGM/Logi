#include "base/VulkanInstance.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class HelloTriangle {
public:
	HelloTriangle() = default;

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
		logi::InstanceConfiguration instance_config{};
		instance_config.extensions = getRequiredExtensions();

		instance = logi::VulkanInstance(app_info, instance_config);
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
		const std::vector<logi::VulkanDevice>& devices = instance.devices();

		for (auto it = devices.begin(); it != devices.end(); ++it) {
			if (it->properties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
				gpu = *it;
				break;
			}
		}
	}

	void initializeDevice() {
		// Configure queue families and initialize device.
		std::vector<logi::QueueFamilyProperties> family_properties = gpu.queueFamilyProperties(surface);

		for (auto it = family_properties.begin(); it != family_properties.end(); ++it) {
			if (it->queue_flags | vk::QueueFlagBits::eGraphics) {
				it->configure(1);
				break;
			}
		}

		gpu.initialize({}, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, family_properties);
		program_manager = gpu.getProgramManager();
		allocation_manager = gpu.getAllocationManager();
	}

	void createRenderPass() {
		logi::RenderPassLayout rp_layout(1u, 1u);
		vk::AttachmentDescription* attachment_desc = rp_layout.getAttachmentDescription(0u);

		// attachment_desc->format = swapChainImageFormat; TODO
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
		logi::Shader vertex_shader = program_manager.loadShader("./resources/shaders/triangle_vert.spv", vk::ShaderStageFlagBits::eVertex);
		logi::Shader fragment_shader = program_manager.loadShader("./resources/shaders/triangle_frag.spv", vk::ShaderStageFlagBits::eFragment);
		logi::PipelineLayout pipeline_layout = program_manager.createPipelineLayout({ vertex_shader, fragment_shader });

		logi::PipelineState state;

		// INPUT ASSEMBLY STATE
		state.input_assembly.primitive_restart_enable = false;
		state.input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

		// VIEWPORT STATE
		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = 100; // TODO swapChainExtent.width;
		viewport.height = 100; // TODO swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D(0, 0);
		scissor.extent = vk::Extent2D(100, 100); // TODO swapChainExtent;
		
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
		pipeline = program_manager.createGraphicalPipelines(create_info)[0];
	}

	void start() {
		initWindow();
		createInstance();
		initSurface();
		selectDevice();
		initializeDevice();
		createRenderPass();
		createGraphicPipeline();
	}

private:
	GLFWwindow* window;
	vk::SurfaceKHR surface;

	static const uint32_t width = 800u;
	static const uint32_t height = 600u;

	logi::VulkanInstance instance{};
	logi::VulkanDevice gpu{};
	logi::ProgramManager program_manager{};
	logi::AllocationManager allocation_manager{};
	logi::RenderPass render_pass;
	logi::GraphicalPipeline pipeline;
};



int main() {
	HelloTriangle example;
	example.start();
	
	system("pause");
}
