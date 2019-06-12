#ifndef EXAMPLE_HELLO_TRIANGLE_H
#define EXAMPLE_HELLO_TRIANGLE_H
#include "base/ExampleBase.h"

class HelloTriangle : public ExampleBase {
public:

	void createRenderPass();

	void createGraphicalPipeline();

	void createFrameBuffers();

	void recordCommandBuffers();

  void initialize() override;

  void draw() override;

private:
	logi::RenderPass render_pass;
	logi::GraphicalPipeline graphical_pipeline;
	std::vector<logi::Framebuffer> framebuffers;
};

#endif