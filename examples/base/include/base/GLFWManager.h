#ifndef BASE_GLFW_MANAGER_H
#define BASE_GLFW_MANAGER_H

#include <GLFW/glfw3.h>
#include <vector>
#include "base/Monitor.h"
#include "base/Window.h"
#include "base/PhysicalDevice.h"

class GLFWManager {
public:
	GLFWManager() {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW.");
		}
	}

	GLFWManager(const GLFWManager&) = delete;
	GLFWManager& operator=(const GLFWManager&) = delete;
	GLFWManager(GLFWManager&&) = delete;
	GLFWManager& operator=(GLFWManager&&) = delete;

	static GLFWManager& instance() {
		static GLFWManager instance;

		return instance;
	}

	std::vector<Monitor> getMonitors() const;

	Monitor getPrimaryMonitor() const;

	void pollEvents() const;

    template <typename... Args>
    Window createWindow(Args... args) {
		return Window(args...);
    }

#ifdef GLFW_INCLUDE_VULKAN
	bool vulkanSupported() const;

	std::vector<const char*> getRequiredInstanceExtensions() const;
#endif
};

#endif // BASE_GLFW_MANAGER_H
