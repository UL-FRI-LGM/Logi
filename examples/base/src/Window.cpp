#include "base/Window.h"

Window::Window(const std::string_view& title, const int32_t width, const int32_t height, std::optional<Monitor> monitor, std::optional<Window> window)
    : data_(nullptr) {

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* glfw_window = glfwCreateWindow(width, height, title.data(), 
		                                       monitor.has_value() ? monitor.value().glfwHandle() : nullptr,
		                                       window.has_value() ? window.value().glfwHandle() : nullptr);
	glfwDefaultWindowHints();

    if (glfw_window == nullptr) {
		throw std::runtime_error("Failed to create GLFW window.");
    }

	data_ = std::make_shared<WindowData>(glfw_window);

	glfwSetWindowUserPointer(glfw_window, this->data_.get());

	glfwSetWindowCloseCallback(data_->window, &Window::onClose);
	glfwSetWindowFocusCallback(data_->window, &Window::onFocus);
	glfwSetWindowIconifyCallback(data_->window, &Window::onIconify);
	glfwSetWindowPosCallback(data_->window, &Window::onPositionChanged);
	glfwSetWindowRefreshCallback(data_->window, &Window::onRefresh);
	glfwSetWindowSizeCallback(data_->window, &Window::onResize);
}

Window::Window() : data_(std::make_shared<WindowData>(nullptr)) {}

bool Window::isMinimized() const {
    if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
    }

	int32_t width = 0;
	int32_t height = 0;
	glfwGetWindowSize(data_->window, &width, &height);
	return width == 0 && height == 0;
}

bool Window::shouldClose() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	return glfwWindowShouldClose(data_->window);
}

void Window::setShouldClose(const bool should_close) const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwSetWindowShouldClose(data_->window, should_close ? 1 : 0);
}

void Window::setTitle(const std::string_view& title) const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwSetWindowTitle(data_->window, title.data());
}

std::pair<int32_t, int32_t> Window::getPosition() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	std::pair<int32_t, int32_t> position;
	glfwGetWindowPos(data_->window, &position.first, &position.second);
	return position;
}

void Window::setPosition(const int32_t x, const int32_t y) const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwSetWindowPos(data_->window, x, y);
}

std::pair<int32_t, int32_t> Window::getSize() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	std::pair<int32_t, int32_t> size;
	glfwGetWindowSize(data_->window, &size.first, &size.second);
	return size;
}

void Window::setSize(const int32_t x, const int32_t y) const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwSetWindowSize(data_->window, x, y);
}

std::pair<int32_t, int32_t> Window::getFramebufferSize() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	std::pair<int32_t, int32_t> size;
	glfwGetFramebufferSize(data_->window, &size.first, &size.second);
	return size;
}

std::tuple<int32_t, int32_t, int32_t, int32_t> Window::getFrameSize() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	std::tuple<int32_t, int32_t, int32_t, int32_t> size;
	glfwGetWindowFrameSize(data_->window, &std::get<0>(size), &std::get<1>(size), &std::get<2>(size), &std::get<3>(size));
	return size;
}

void Window::destroy() const {
	if (!data_->destroyed) {
		glfwDestroyWindow(data_->window);
		data_->destroyed = true;
	}
}

bool Window::isDestroyed() const {
	return data_->destroyed;
}

void Window::setSizeLimits(const int32_t min_width, const int32_t min_height, const int32_t max_width, const int32_t max_height) const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwSetWindowSizeLimits(data_->window, min_width, min_height, max_width, max_height);
}

void Window::setAspectRatio(const int32_t numerator, const int32_t denominator) const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwSetWindowAspectRatio(data_->window, numerator, denominator);
}

void Window::iconify() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwIconifyWindow(data_->window);
}

void Window::restore() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwRestoreWindow(data_->window);
}

void Window::maximize() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwMaximizeWindow(data_->window);
}

void Window::show() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwShowWindow(data_->window);
}

void Window::hide() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwHideWindow(data_->window);
}

void Window::focus() const {
	if (data_->destroyed) {
		throw std::runtime_error("Tried to use destroyed window.");
	}

	glfwFocusWindow(data_->window);
}

void Window::setOnCloseCallback(const std::string& id, const std::function<void(Window)>& callback) const {
	data_->on_close_callbacks.addCallback(id, callback);
}

void Window::removeOnCloseCallback(const std::string& id) const {
	data_->on_close_callbacks.removeCallback(id);
}

void Window::clearOnCloseCallbacks() const {
	data_->on_close_callbacks.clearCallbacks();
}

void Window::setOnFocusCallback(const std::string& id, const std::function<void(Window, bool)>& callback) const {
	data_->on_focus_callbacks.addCallback(id, callback);
}
void Window::removeOnFocusCallback(const std::string& id) const {
	data_->on_focus_callbacks.removeCallback(id);
}
void Window::clearOnFocusCallbacks() const {
	data_->on_focus_callbacks.clearCallbacks();
}

void Window::setOnIconifyCallback(const std::string& id, const std::function<void(Window, bool)>& callback) const {
	data_->on_iconify_callbacks.addCallback(id, callback);
}

void Window::removeOnIconifyCallback(const std::string& id) const {
	data_->on_iconify_callbacks.removeCallback(id);
}

void Window::clearOnIconifyCallbacks() const {
	data_->on_iconify_callbacks.clearCallbacks();
}

void Window::setOnPositionChangedCallback(const std::string& id, const std::function<void(Window, int32_t, int32_t)>& callback) const {
	data_->on_position_changed_callbacks.addCallback(id, callback);
}

void Window::removeOnPositionChangedCallback(const std::string& id) const {
	data_->on_position_changed_callbacks.removeCallback(id);
}
void Window::clearOnPositionChangedCallbacks() const {
	data_->on_position_changed_callbacks.clearCallbacks();
}

void Window::setOnRefreshCallback(const std::string& id, const std::function<void(Window)>& callback) const {
	data_->on_refresh_callbacks.addCallback(id, callback);
}

void Window::removeOnRefreshCallback(const std::string& id) const {
	data_->on_refresh_callbacks.removeCallback(id);
}

void Window::clearOnRefreshCallbacks() const {
	data_->on_refresh_callbacks.clearCallbacks();
}

void Window::setOnResizeCallback(const std::string& id, const std::function<void(Window, int32_t, int32_t)>& callback) const {
	data_->on_resize_callbacks.addCallback(id, callback);
}

void Window::removeOnResizeCallback(const std::string& id) const {
	data_->on_resize_callbacks.removeCallback(id);
}

void Window::clearOnResizeCallbacks() const {
	data_->on_resize_callbacks.clearCallbacks();
}

#ifdef GLFW_INCLUDE_VULKAN

vk::ResultValue<vk::SurfaceKHR> Window::createWindowSurface(const vk::Instance& instance,
    std::optional<const vk::AllocationCallbacks> allocation_callbacks) {
	vk::SurfaceKHR surface;
	vk::Result result = vk::Result(glfwCreateWindowSurface((VkInstance) instance,
		data_->window,
		allocation_callbacks.has_value() ? (VkAllocationCallbacks*) &allocation_callbacks.value() : nullptr,
		(VkSurfaceKHR*) &surface));

	return vk::ResultValue<vk::SurfaceKHR>(result, surface);
}

#endif

GLFWwindow* Window::glfwHandle() const {
	return data_->window;
}

void Window::onClose(GLFWwindow* glfw_window) {
    auto* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfw_window));
	data->on_close_callbacks.triggerCallbacks(Window(data->getSharedPtr()));
}

void Window::onFocus(GLFWwindow* glfw_window, const int32_t focused) {
	auto* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfw_window));
    data->on_focus_callbacks.triggerCallbacks(Window(data->getSharedPtr()), focused != 0);
}

void Window::onIconify(GLFWwindow* glfw_window, const int32_t iconified) {
	auto* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfw_window));
	data->on_iconify_callbacks.triggerCallbacks(Window(data->getSharedPtr()), iconified != 0);
}

void Window::onPositionChanged(GLFWwindow* glfw_window, const int32_t x, const int32_t y) {
	auto* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfw_window));
    data->on_position_changed_callbacks.triggerCallbacks(Window(data->getSharedPtr()), x, y);
}

void Window::onRefresh(GLFWwindow* glfw_window) {
	auto* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfw_window));
    data->on_refresh_callbacks.triggerCallbacks(Window(data->getSharedPtr()));
}

void Window::onResize(GLFWwindow* glfw_window, const int32_t width, const int32_t height) {
	auto* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfw_window));
	data->on_resize_callbacks.triggerCallbacks(Window(data->getSharedPtr()), width, height);
}

Window::WindowData::WindowData(GLFWwindow* window)
    : window(window), destroyed(window == nullptr) { }

Window::Window(std::shared_ptr<WindowData> data) : data_(std::move(data)) {
}
