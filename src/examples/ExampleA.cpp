/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "examples/ExampleA.h"
#include "program_management/ProgramManager.h"
#include <vector>
#include <iostream>
#include "base/VulkanDevice.h"
#include "util/helpers.h"
#include "base/Types.h"
#include "descriptors/DecriptorsUpdate.h"
#include "examples/lodepng.h"

namespace vkr {

const int WIDTH = 3200;
const int HEIGHT = 2400;
const int WORKGROUP_SIZE = 32;

ExampleA::ExampleA(std::vector<char *>& global_extensions, std::vector<char *>& device_extensions, vk::PhysicalDeviceFeatures& features) : RendererBase(global_extensions), gpu_(nullptr) {

	auto scoreDevice = [&device_extensions, &features](const VulkanDevice* device) {
		// Device must support all requested extensions.
		for (char* extension : device_extensions) {
			if (!device->extensionSupported(extension)) {
				return -1;
			}
		}

		// Check if the device supports all requested features.
		if (!hasRequestedFeatures(device->features(), features)) {
			return -1;
		}

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
	gpu_->initialize(features, device_extensions, QueueConfig(0, 1, 0));
}

void ExampleA::run() {
	// Load programs.
	ProgramManager* prog_manager = gpu_->getProgramManager();
	prog_manager->loadPrograms("./resources/shaders");

	// Retrieve resource managers.
	gpu_->createDescriptorPool(prog_manager->getDescriptorsCount());
	DescriptorPool* desc_pool = gpu_->getDescriptorPool();
	AllocationManager* alloc_manager = gpu_->getAllocationManager();

	QueueFamily* compute_family = gpu_->getComputeFamily();
	Queue* compute_queue = compute_family->getQueue(0);
	CommandPool* cmd_pool = compute_family->createCommandPool(false, false);

	// Fetch compute pipeline
	ComputePipeline* compute_pipeline = prog_manager->getComputePipeline(prog_manager->getComputePipelineId("compute_test"));
	const PipelineLayout* pipeline_layout = compute_pipeline->getPipelineLayout();

	// Allocate buffer and descriptor set.
	Buffer* buffer = alloc_manager->allocateBuffer(BufferConfiguration(WIDTH * HEIGHT * sizeof(Pixel), vk::BufferUsageFlagBits::eStorageBuffer, MemoryUsage::GPU_TO_CPU));
	DescriptorSet* desc_set = desc_pool->createDescriptorSet(pipeline_layout->getDescriptorSetLayout(0));

	// Record and execute descriptors updates.
	DescriptorsUpdate desc_update;
	desc_update.writeBufferToDescriptorSet(desc_set, 0, 0, buffer, 0, WIDTH * HEIGHT * sizeof(Pixel));
	gpu_->executeDescriptorsUpdate(desc_update);

	// Record command buffer.
	std::unique_ptr<PrimaryCommandBuffer> cmd_buffer = cmd_pool->allocatePrimaryCommandBuffer();
	cmd_buffer->beginCommandBuffer(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	cmd_buffer->bindPipeline(compute_pipeline->getVkHandle(), vk::PipelineBindPoint::eCompute);
	cmd_buffer->bindDescriptorSets(pipeline_layout->getVkHandle(), vk::PipelineBindPoint::eCompute, 0, { desc_set->getVkDescriptorSet() }, {});
	cmd_buffer->computeDispatch((uint32_t)ceil(WIDTH / float(WORKGROUP_SIZE)), (uint32_t)ceil(HEIGHT / float(WORKGROUP_SIZE)), 1);
	cmd_buffer->endCommandBuffer();

	// Submit command buffer and wait for it to finish
	vk::SubmitInfo submit_info{};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer->getVkHandle();

	Fence fence(gpu_->getLogicalDeviceHandle(), false);
	compute_queue->submit({ submit_info }, &fence);
	fence.wait(100000000000);

	// Retrieve storage buffer from the device.
	std::vector<unsigned char> data = buffer->getData();
	Pixel* pixels = reinterpret_cast<Pixel*>(data.data());

	// Save image
	std::vector<unsigned char> image;
	image.reserve(WIDTH * HEIGHT * 4);

	for (int i = 0; i < data.size() / sizeof(Pixel); i++) {
		image.push_back((unsigned char)(255.0f * (pixels[i].r)));
		image.push_back((unsigned char)(255.0f * (pixels[i].g)));
		image.push_back((unsigned char)(255.0f * (pixels[i].b)));
		image.push_back((unsigned char)(255.0f * (pixels[i].a)));
	}

	unsigned error = lodepng::encode("mandelbrot.png", image, WIDTH, HEIGHT);
}

void ExampleA::runExample() {
	std::vector<char*> global_extensions;
	std::vector<char*> device_extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	vk::PhysicalDeviceFeatures features;

	ExampleA example(global_extensions, device_extensions, features);
	example.run();

	std::cout << "success" << std::endl;
}

}