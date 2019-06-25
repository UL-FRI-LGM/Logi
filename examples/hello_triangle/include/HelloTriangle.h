#ifndef EXAMPLE_HELLO_TRIANGLE_H
#define EXAMPLE_HELLO_TRIANGLE_H
#include "base/ExampleBase.h"

class HelloTriangle : public ExampleBase {
 protected:
  void createRenderPass();

  void createGraphicalPipeline();

  void createFrameBuffers();

  void recordCommandBuffers();

  void initialize() override;

  void draw() override;

 private:
  logi::RenderPass renderPass_;
  logi::PipelineLayout pipelineLayout_;
  logi::Pipeline pipeline_;
  std::vector<logi::Framebuffer> framebuffers_;
};

#endif