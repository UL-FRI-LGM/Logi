#include "base/ExampleBase.h"


ExampleBase::ExampleBase(const ExampleConfiguration& config) : config_(config) {}

void ExampleBase::run() {
	initWindow();
	createInstance();
	initSurface();
	selectDevice();
	initializeDevice();
	initializeSwapChain();
	buildSyncObjects();
	initializeCommandBuffers();
	initialize();
	mainLoop();
}

VkBool32 ExampleBase::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj_type, uint64_t obj,
                                    size_t location, int32_t code, const char* layer_prefix, const char* msg, void* user_data) {
  std::cout << "validation layer: " << msg << std::endl;
  return VK_FALSE;
}

void ExampleBase::initWindow() {
	window = GLFWManager::instance().createWindow(config_.window_title, config_.window_width, config_.window_height);
}

void ExampleBase::createInstance() {
	logi::InstanceConfig instance_config;

  // Set extensions.
	instance_config.extensions = GLFWManager::instance().getRequiredInstanceExtensions();
	instance_config.extensions.insert(instance_config.extensions.end(), config_.instance_extensions.begin(), config_.instance_extensions.end());
	instance_config.extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	instance_config.validation_layers = config_.validation_layers;
  
	vk_instance = logi::VulkanInstance(instance_config);
	vk_instance.setupDebugCallback(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning, debugCallback);
}

void ExampleBase::initSurface() {
	surface = window.createWindowSurface(vk_instance.getVkHandle()).value;
}

void ExampleBase::selectDevice() {
	// Select GPU
	const std::vector<logi::PhysicalDevice>& devices = vk_instance.devices();

	for (const auto device : devices) {
		vk::PhysicalDeviceType type = device.getProperties().deviceType;

		if (type == vk::PhysicalDeviceType::eDiscreteGpu) {
      // If discrete gpu is found select it immediately.
			physical_device = device;
			return;
		} else if (type == vk::PhysicalDeviceType::eIntegratedGpu || type == vk::PhysicalDeviceType::eVirtualGpu) {
			physical_device = device;
		}
	}

	assert(false);
}

void ExampleBase::initializeDevice() {
	std::vector<logi::QueueFamilyProperties> family_properties = physical_device.queueFamilyProperties();
	std::vector<logi::QueueFamilyConfig> configurations;

	// Search for graphical queue family.
	uint32_t graphical_index = std::numeric_limits<uint32_t>::max();

	for (const logi::QueueFamilyProperties& properties : family_properties) {
		if (properties.queue_flags | vk::QueueFlagBits::eGraphics) {
			graphical_index = properties.family_index;
			configurations.emplace_back(logi::QueueFamilyConfig(properties, 1u));
			break;
		}
	}

	if (graphical_index == std::numeric_limits<uint32_t>::max()) {
		throw std::runtime_error("Failed to find graphical queue.");
	}

	uint32_t present_index = std::numeric_limits<uint32_t>::max();

  // Add presentation family if the example is not headless.
	if (!physical_device.supportsSurfacePresent(surface, configurations[0].properties)) {

		for (const logi::QueueFamilyProperties& properties : family_properties) {
			if (physical_device.supportsSurfacePresent(surface, properties)) {
				configurations.emplace_back(logi::QueueFamilyConfig(properties, 1u));
				present_index = properties.family_index;
				break;
			}
		}

		if (present_index == std::numeric_limits<uint32_t>::max()) {
			throw std::runtime_error("Failed to find queue family that supports presentation.");
		}
	} else {
		present_index = graphical_index;
	}

  std::vector<const char*> extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  extensions.insert(extensions.end(), config_.device_extensions.begin(), config_.device_extensions.end());
  gpu = physical_device.createLogicalDevice(logi::LogicalDeviceConfig(configurations, extensions));

	present_family = gpu.getQueueFamily(present_index);
	present_queue = present_family.getQueue(0u);
	render_family = gpu.getQueueFamily(graphical_index);
	render_queue = render_family.getQueue(0u);
}

void ExampleBase::initializeSwapChain() {
	const std::vector<logi::QueueFamily>& queue_families = gpu.getQueueFamilies();

  if (queue_families.size() == 1u) {
		swap_chain = gpu.createSwapchain(surface, present_family.configuration().properties.family_index);
	} else {
	  std::vector<uint32_t> concurrent_indices;
    for (auto family : queue_families) {
		  concurrent_indices.push_back(family.configuration().properties.family_index);
    }

		swap_chain = gpu.createSwapchain(surface, present_family.configuration().properties.family_index, concurrent_indices);
	}

  swap_chain.create({ static_cast<uint32_t>(window.getFramebufferSize().first), 
					           static_cast<uint32_t>(window.getFramebufferSize().second) }, false);
}

void ExampleBase::initializeCommandBuffers() {
	cmd_pool = render_family.createCommandPool();
  
  for (size_t i = 0; i < swap_chain.getImageViews().size(); i++) {
	  cmd_buffers.emplace_back(cmd_pool.createPrimaryCommandBuffer());
  }
}

void ExampleBase::buildSyncObjects() {
	for (size_t i = 0; i < config_.max_frames_in_flight; i++) {
		image_available_s_.emplace_back(gpu.createSemaphore());
		render_finished_s_.emplace_back(gpu.createSemaphore());
		in_flight_f_.emplace_back(gpu.createFence(vk::FenceCreateFlagBits::eSignaled));
	}
}

void ExampleBase::drawFrame() {
  // Wait if drawing is still in progress.
	in_flight_f_[current_frame_].wait(std::numeric_limits<uint64_t>::max());
	in_flight_f_[current_frame_].reset();

  // Acquire next image.
	const uint32_t image_index = swap_chain.acquireNextImage(image_available_s_[current_frame_]).value;

	static const vk::PipelineStageFlags wait_stages{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

  // Example draw.
	draw();

	vk::SubmitInfo submit_info;
	submit_info.pWaitDstStageMask = &wait_stages;
	submit_info.pWaitSemaphores = &image_available_s_[current_frame_].getVkHandle();
	submit_info.waitSemaphoreCount = 1u;

	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffers[image_index].getVkHandle();

	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &render_finished_s_[current_frame_].getVkHandle();
	render_queue.submit({ submit_info }, in_flight_f_[current_frame_]);

  // Present image.
	swap_chain.queuePresent(present_queue, render_finished_s_[current_frame_]);
	current_frame_ = (current_frame_ + 1) % config_.max_frames_in_flight;
}

void ExampleBase::mainLoop() {
	while (!window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}

	gpu.waitIdle();
}
