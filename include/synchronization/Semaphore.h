#ifndef SYNCHRONIZATION_SEMAPHORE_H_
#define SYNCHRONIZATION_SEMAPHORE_H_

#include <vulkan/vulkan.hpp>

namespace vkr {

class Semaphore {
public:
	/**
	 * @brief Allocate the semaphore from the given device.
	 *
	 * @param device Device associated with the semaphore.
	 * @param wait_stages Stages at which the semaphore will wait.
	 */
	Semaphore(const vk::Device& device, const vk::PipelineStageFlags& wait_stages);

	/**
	 * @brief Retrieve Vulkan semaphore handle.
	 *
	 * @return Vulkan semaphore handle.
	 */
	const vk::Semaphore& getVkHandle() const;

	/**
	 * @brief Set stages at which the semaphore will wait for signal. Overwrites current configuration.
	 *
	 * @param wait_stages Stages at which the semaphore should wait for signal.
	 */
	void setWaitStages(const vk::PipelineStageFlags& wait_stages);

	/**
	 * @brief Get stages at which the semaphore will wait for signal.
	 *
	 * @return Stages at which the semaphore should wait for signal.
	 */
	const vk::PipelineStageFlags& getWaitStages() const;

	/**
	 * @brief Free Vulkan semaphore.
	 */
	~Semaphore();

private:
	vk::Device device_; ///< Vulkan logical device.
	vk::Semaphore semaphore_; ///< Vulkan semaphore handle.
	vk::PipelineStageFlags wait_stages_; ///< Stages at which the semaphore will wait.
};

}

#endif //! SYNCHRONIZATION_SEMAPHORE_H_