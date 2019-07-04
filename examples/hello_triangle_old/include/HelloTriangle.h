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
  logi::RenderPass renderPass;
  logi::PipelineLayout pipelineLayout;
  logi::GraphicalPipeline graphicalPipeline;
  std::vector<logi::Framebuffer> framebuffers;
};

#endif