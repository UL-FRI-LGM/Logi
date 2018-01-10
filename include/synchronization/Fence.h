#ifndef SYNCHRONIZATION_FENCE_H_
#define SYNCHRONIZATION_FENCE_H_

#include <vulkan/vulkan.hpp>

namespace vkr {

class Fence {
public:
	/**
	 * @brief Allocate the fence from the given device.
	 *
	 * @param device Device associated with the fence.
	 * @param initially_signaled If true the fence will be initialized with signaled state.
	 */
	Fence(const vk::Device& device, bool initially_signaled);

	/**
	 * @brief Return true if the fence is signaled.
	 *
	 * @return True if the fence is signaled.
	 */
	bool getStatus();

	/**
	 * @brief Resets state to unsignaled.
	 */
	void reset();

	/**
	 * @brief Wait for the fence to be signaled.
	 *
	 * @param timeout Timeout time in nanoseconds.
	 * @return Vulkan operation result.
	 */
	vk::Result wait(uint64_t timeout);

	/**
	 * @brief Wait for the fences to be signaled.
	 * @note All fences must originate from the same device!
	 *
	 * @param timeout Timeout time in nanoseconds.
	 * @param wait_all If true the operation will block until all fences are signaled, otherwise until the first fence is signaled.
	 * @return Vulkan operation result.
	 */
	static vk::Result wait(const std::vector<Fence*>& fences, bool wait_all, uint64_t timeout);

	/**
	 * @brief Retrieve Vulkan fence handle.
	 *
	 * @return Vulkan fence handle.
	 */
	const vk::Fence& getVkHandle() const;

	/**
	 * @brief Free Vulkan fence.
	 */
	~Fence();

private:
	vk::Device device_; ///< Vulkan logical device.
	vk::Fence fence_; ///< Vulkan fence handle.
};

}

#endif //! SYNCHRONIZATION_FENCE_H_