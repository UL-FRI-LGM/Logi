#ifndef BASE_GLFW_MONITOR_H
#define BASE_GLFW_MONITOR_H

#include <GLFW/glfw3.h>
#include <utility>
#include <vector>

class Monitor {
public:
    explicit Monitor(GLFWmonitor* monitor);

	std::pair<int32_t, int32_t> getPosition() const;

	std::pair<int32_t, int32_t> getPhysicalSize() const;

	std::string_view getName() const;

	bool isPrimary() const;

	GLFWmonitor* glfwHandle() const;

private:
	GLFWmonitor* monitor_;
};

#endif // BASE_GLFW_MONITOR_H
