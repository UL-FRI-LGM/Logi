#include "examples/ExampleGraphics.h"
#include <iostream>
#include "util/helpers.h"
#include "base/Types.h"

namespace vkr {

GraphicsExample::GraphicsExample() : RendererBase(), gpu_(nullptr) {

}

void GraphicsExample::run() {
	initWindow();
	initVulkan();
	initializeSwapchain();
	mainLoop();
	cleanup();
}

void GraphicsExample::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(kWidth, kHeight, "Vulkan", nullptr, nullptr);
}

void GraphicsExample::initVulkan() {
	uint32_t count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);
	std::vector<const char*> extensions;
	extensions.assign(glfw_extensions, glfw_extensions + count);

	// Initialize Vulkan instance.
	initVulkanInstance(extensions);

	auto scoreDevice = [](const VulkanDevice* device) {
		int32_t score = 0;
		// Prefer discrete gpu-s.
		if (device->properties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
			score += 100000;
		}

		// Score based on the device memory.
		std::vector<vk::MemoryHeap> heaps = std::vector<vk::MemoryHeap>(device->memoryProperties().memoryHeaps, device->memoryProperties().memoryHeaps + device->memoryProperties().memoryHeapCount);

		for (const vk::MemoryHeap& heap : heaps) {
			if (heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
				score += heap.size / 1048576;
				break;
			}
		}

		return score;
	};

	// Retrieve devices that match the scoring function criteria.
	std::vector<std::pair<int32_t, VulkanDevice*>> devices = retrieveDevices(scoreDevice);

	// Make user select the device.
	std::cout << "Select the device: " << std::endl;
	for (size_t i = 0; i < devices.size(); i++) {
		std::cout << i << ": " << devices[i].second->properties().deviceName << std::endl;
	}

	size_t selection;
	while (true) {
		std::cin >> selection;

		if (selection < devices.size()) {
			break;
		}
	}

	// Select and initialize the device.
	gpu_ = devices[selection].second;
	gpu_->initialize({}, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, QueueConfig(1, 0, 0));
}


void GraphicsExample::initializeSwapchain() {
	VkSurfaceKHR vk_surface;
	if (glfwCreateWindowSurface(static_cast<VkInstance>(getVulkanInstance()), window, nullptr, &vk_surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	surface = vk::SurfaceKHR(vk_surface);
	swapchain.connect(getVulkanInstance(), gpu_);
	swapchain.init(surface);
	uint32_t actual_width = kWidth;
	uint32_t actual_heigth = kHeight;
	swapchain.create(actual_width, actual_heigth);
}

void GraphicsExample::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void GraphicsExample::cleanup() {
	swapchain.cleanup();
	getVulkanInstance().destroySurfaceKHR(surface);
	glfwDestroyWindow(window);
	glfwTerminate();
}



}