#include "examples/ExampleGraphics.h"
#include "program/layout/PipelineState.h"
#include <iostream>
#include <chrono>
#include "program/render_pass/RenderPassLayout.h"
#include "base/ManagedResource.h"

void delete_fn(const vk::Image& image) {
	std::cout << "Delete image" << std::endl;
}

int main() {
	vk::Device device;
	vk::Image image;
	vkr::ManagedResource<vk::Image, vk::Device, vk::DispatchLoaderStatic, &vk::Device::destroyImage> m_image(device, image);
	m_image.destroy();

	{
		vkr::GraphicsExample example;
		example.run();
	}

	vkr::RenderPassLayout render_pass_layout(3, 2);
	render_pass_layout.buildCreateInfo();

	
	
	
	//vkr::PipelineState state;
	//vkr::PipelineLayout pipeline_layout;

	system("pause");
}