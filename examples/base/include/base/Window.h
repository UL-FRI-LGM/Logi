#ifndef BASE_GLFW_WINDOW_H
#define BASE_GLFW_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include "base/Monitor.h"
#include <vulkan/vulkan.hpp>

template <typename IdentifierType, typename ...CallbackArgs>
class CallbackCollection {
public:

	CallbackCollection() = default;

	void addCallback(const IdentifierType& id, const std::function<void(CallbackArgs...)>& callback) {
		callbacks_[id] = callback;
	}

	void removeCallback(const IdentifierType& id) {
		callbacks_.erase(id);
	}

	void clearCallbacks() {
		callbacks_.clear();
	}

	void triggerCallbacks(CallbackArgs... args) const {
		for (const auto& callback : callbacks_) {
			callback.second(args...);
		}
	}

private:
	std::map<IdentifierType, std::function<void(CallbackArgs...)>> callbacks_;
};


class Window {
public:
	Window();

	friend class GLFWManager;

	bool isMinimized() const;

	bool shouldClose() const;

	void setShouldClose(bool should_close) const;

	void setTitle(const std::string_view& title) const;

	std::pair<int32_t, int32_t> getPosition() const;

	void setPosition(int32_t x, int32_t y) const;

	std::pair<int32_t, int32_t> getSize() const;

	void setSize(int32_t x, int32_t y) const;

	std::pair<int32_t, int32_t> getFramebufferSize() const;

	std::tuple<int32_t, int32_t, int32_t, int32_t> getFrameSize() const;

	void destroy() const;

	bool isDestroyed() const;

	void setSizeLimits(int32_t min_width, int32_t min_height, int32_t max_width, int32_t max_height) const;

	void setAspectRatio(int32_t numerator, int32_t denominator) const;

	void iconify() const;

	void restore() const;

	void maximize() const;

	void show() const;

	void hide() const;

	void focus() const;

	void setOnCloseCallback(const std::string& id, const std::function<void(Window)>& callback) const;

	void removeOnCloseCallback(const std::string& id) const;

	void clearOnCloseCallbacks() const;


	void setOnFocusCallback(const std::string& id, const std::function<void(Window, bool)>& callback) const;

	void removeOnFocusCallback(const std::string& id) const;

	void clearOnFocusCallbacks() const;


	void setOnIconifyCallback(const std::string& id, const std::function<void(Window, bool)>& callback) const;

	void removeOnIconifyCallback(const std::string& id) const;

	void clearOnIconifyCallbacks() const;


	void setOnPositionChangedCallback(const std::string& id, const std::function<void(Window, int32_t, int32_t)>& callback) const;

	void removeOnPositionChangedCallback(const std::string& id) const;

	void clearOnPositionChangedCallbacks() const;


	void setOnRefreshCallback(const std::string& id, const std::function<void(Window)>& callback) const;

	void removeOnRefreshCallback(const std::string& id) const;

	void clearOnRefreshCallbacks() const;


	void setOnResizeCallback(const std::string& id, const std::function<void(Window, int32_t, int32_t)>& callback) const;

	void removeOnResizeCallback(const std::string& id) const;

	void clearOnResizeCallbacks() const;

#ifdef GLFW_INCLUDE_VULKAN
	vk::ResultValue<vk::SurfaceKHR> createWindowSurface(const vk::Instance& instance, std::optional<const vk::AllocationCallbacks> allocation_callbacks = nullptr);
#endif

	GLFWwindow* glfwHandle() const;

protected:

	Window(const std::string_view& title, int32_t width, int32_t height, std::optional<Monitor> monitor = {}, std::optional<Window> window = {});

	static void onClose(GLFWwindow* glfw_window);

	static void onFocus(GLFWwindow* glfw_window, int32_t focused);

	static void onIconify(GLFWwindow* glfw_window, int32_t iconified);

	static void onPositionChanged(GLFWwindow* glfw_window, int32_t x, int32_t y);

	static void onRefresh(GLFWwindow* glfw_window);

	static void onResize(GLFWwindow* glfw_window, int32_t width, int32_t height);

private:
    struct WindowData : std::enable_shared_from_this<WindowData> {
        explicit WindowData(GLFWwindow* window);

		GLFWwindow* window;
		bool destroyed;

		CallbackCollection<std::string, Window> on_close_callbacks;
		CallbackCollection<std::string, Window, bool> on_focus_callbacks;
		CallbackCollection<std::string, Window, bool> on_iconify_callbacks;
		CallbackCollection<std::string, Window, int32_t, int32_t> on_position_changed_callbacks;
		CallbackCollection<std::string, Window> on_refresh_callbacks;
		CallbackCollection<std::string, Window, int32_t, int32_t> on_resize_callbacks;

        std::shared_ptr<WindowData> getSharedPtr() {
            return shared_from_this();
        }
    };

    explicit Window(std::shared_ptr<WindowData> data);

	std::shared_ptr<WindowData> data_;
};


#endif