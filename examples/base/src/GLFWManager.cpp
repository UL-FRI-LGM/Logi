#include "base/GLFWManager.h"


std::vector<Monitor> GLFWManager::getMonitors() const {
	int32_t count = 0;
	GLFWmonitor** glfw_monitors = glfwGetMonitors(&count);

	std::vector<Monitor> monitors;
    for (int32_t i = 0; i < count; i++) {
		monitors.emplace_back(glfw_monitors[i]);
    }

	return monitors;
}

Monitor GLFWManager::getPrimaryMonitor() const {
	return Monitor(glfwGetPrimaryMonitor());
}

void GLFWManager::pollEvents() const {
	glfwPollEvents();
}

#ifdef GLFW_INCLUDE_VULKAN
bool GLFWManager::vulkanSupported() const {
	return glfwVulkanSupported() != 0;
}

std::vector<const char*> GLFWManager::getRequiredInstanceExtensions() const {
	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);
	return std::vector<const char*>(extensions, extensions + count);
}
#endif

