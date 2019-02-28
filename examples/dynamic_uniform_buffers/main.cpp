#include "base/VulkanInstance.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "base/SwapChain.h"
#include <iostream>
#include "base/ExtensionObject.h"


class ExampleBase {
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
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
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
		VkSurfaceKHR vk_surface;

		if (glfwCreateWindowSurface(static_cast<VkInstance>(instance.getVkHandle()), window, nullptr, &vk_surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}

		surface_ = vk::SurfaceKHR(vk_surface);
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
		if (physical_device.supportsSurfacePresent(surface_, configurations[0].properties)) {
			present_family_index = render_family_index;
		}
		else {
			for (const logi::QueueFamilyProperties& properties : family_properties) {
				if (physical_device.supportsSurfacePresent(surface_, properties)) {
					configurations.emplace_back(logi::QueueFamilyConfig(properties, 1u));
					present_family_index = properties.family_index;
					break;
				}
			}

			if (present_family_index == std::numeric_limits<uint32_t>::max()) {
				throw std::runtime_error("Failed to find queue family that supports presentation.");
			}
		}

		gpu = physical_device.createLogicalDevice(logi::LogicalDeviceConfig(configurations, std::vector<char*>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }));
		program_manager = gpu.getProgramManager();
		allocation_manager = gpu.getAllocationManager();
		command_pool = gpu.getQueueFamily(render_family_index).createCommandPool();
		render_queue = gpu.getQueueFamily(render_family_index).getQueue(0u);
		present_queue = gpu.getQueueFamily(present_family_index).getQueue(0u);
	}

	void initializeSwapchain() {
		if (render_family_index == present_family_index) {
			swapchain = gpu.createSwapchain(surface_, present_family_index);
		}
		else {
			swapchain = gpu.createSwapchain(surface_, present_family_index, { render_family_index, present_family_index });
		}

		swapchain.create(swapchain_extent.width, swapchain_extent.height, false);
	}


	void start() {
		initWindow();
		createInstance();
		initSurface();
		selectDevice();
		initializeDevice();
		initializeSwapchain();
	}

private:
	struct Texture {
		logi::Image image{};
		logi::ImageView image_view{};
		logi::Sampler sampler{};
	};

	GLFWwindow* window = nullptr;
	vk::SurfaceKHR surface_;

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


	logi::DescriptorSet graphic_desc_set{};

	uint32_t render_family_index;
	logi::Queue render_queue{};
	uint32_t present_family_index;
	logi::Queue present_queue{};

	logi::SwapChain swapchain{};
	vk::Extent2D swapchain_extent{ width, height };
};

int main() {
	logi::TessellationState tessellation_state;
	tessellation_state.addExtensions(logi::TessellationDomainOriginState());

	{
		ExampleBase example;
		example.start();
	}


	system("pause");
}
