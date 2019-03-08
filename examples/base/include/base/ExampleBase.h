#ifndef BASE_EXAMPLE_BASE_H
#define BASE_EXAMPLE_BASE_H

#define GLFW_INCLUDE_VULKAN
#include <base/GLFWManager.h>
#include "logi/base/VulkanInstance.h"
#include <iostream>

struct ExampleConfiguration {
	std::string window_title = "Example";
	int32_t window_width = 800;
	int32_t window_height = 600;
	size_t max_frames_in_flight = 2u;
	std::vector<const char*> instance_extensions;
	std::vector<const char*> device_extensions;
	std::vector<const char*> validation_layers = { "VK_LAYER_LUNARG_standard_validation", "VK_LAYER_RENDERDOC_Capture" };
};


class ExampleBase {
public:

	explicit ExampleBase(const ExampleConfiguration& config = {});

  void run();

protected:
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj_type,
                                                      uint64_t obj, size_t location, int32_t code,
                                                      const char* layer_prefix, const char* msg, void* user_data);;

  void initWindow();

	void createInstance();

	void initSurface();

	void selectDevice();

	void initializeDevice();

	void initializeSwapChain();

	void initializeCommandBuffers();

	void buildSyncObjects();

	void drawFrame();

	virtual void initialize() = 0;

	virtual void draw() = 0;

  void mainLoop();

protected:
	Window window;
	vk::SurfaceKHR surface;
	logi::QueueFamily present_family;
	logi::QueueFamily render_family;
  logi::Queue present_queue;
	logi::Queue render_queue;
	logi::CommandPool cmd_pool;
	std::vector<logi::PrimaryCommandBuffer> cmd_buffers;

	logi::VulkanInstance vk_instance;
	logi::PhysicalDevice physical_device;
	logi::LogicalDevice gpu;
	logi::SwapChain swap_chain;

private:
	ExampleConfiguration config_;
	std::vector<logi::Fence> in_flight_f_;
	std::vector<logi::Semaphore> image_available_s_;
	std::vector<logi::Semaphore> render_finished_s_;
	size_t current_frame_ = 0u;
};


#endif