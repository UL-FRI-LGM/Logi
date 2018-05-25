/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef EXAMPLE_EXAMPLE_GRAPHICS
#define EXAMPLE_EXAMPLE_GRAPHICS

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <functional>
#include "base/RendererBase.h"
#include "base/SwapChain.h"

namespace vkr {

class GraphicsExample : public RendererBase {
public:
	GraphicsExample();

	void run();

protected:
	void initWindow();

	void initVulkan();


	void initializeSwapchain();

	void mainLoop();

	void cleanup();
private:
	GLFWwindow* window;
	vk::SurfaceKHR surface;
	SwapChain swapchain;

	VulkanDevice* gpu_;
	static const uint32_t kWidth = 800u;
	static const uint32_t kHeight = 600u;
};

}

#endif