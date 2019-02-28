#include "base/Monitor.h"

Monitor::Monitor(GLFWmonitor* monitor)
    : monitor_(monitor) {}

std::pair<int32_t, int32_t> Monitor::getPosition() const {
	std::pair<int32_t, int32_t> position;
	glfwGetMonitorPos(monitor_, &position.first, &position.second);
	return position;
}

std::pair<int32_t, int32_t> Monitor::getPhysicalSize() const {
	std::pair<int32_t, int32_t> size;
	glfwGetMonitorPhysicalSize(monitor_, &size.first, &size.second);
	return size;
}

std::string_view Monitor::getName() const {
	return std::string_view(glfwGetMonitorName(monitor_));
}

bool Monitor::isPrimary() const {
	return monitor_ == glfwGetPrimaryMonitor();
}

GLFWmonitor* Monitor::glfwHandle() const {
	return monitor_;
}

