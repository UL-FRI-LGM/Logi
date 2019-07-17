#ifndef EXAMPLE_TEXTURE_H
#define EXAMPLE_TEXTURE_H
#include <glm/glm.hpp>
#include "base/example_base.h"
#include "stb_image.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

struct Texture {
  logi::VMAImage image;
  logi::ImageView imageView;
  logi::Sampler sampler;
};

class TextureExample : public ExampleBase {
 protected:
  void loadShaders();

  void allocateBuffers();

  void loadTextureImage();

  void updateUniformBuffers();

  void initializeDescriptorSets();

  void createRenderPass();

  void createGraphicalPipeline();

  void createFrameBuffers();

  void recordCommandBuffers();

  void onViewChanged() override;

  void onSwapChainRecreate() override;

  void initialize() override;

  void draw() override;

 private:
  struct {
    glm::mat4 projectionMatrix = glm::mat4(1);
    glm::mat4 modelViewMatrix = glm::mat4(1);
    glm::vec3 viewPos = glm::vec3();
  } ubo_;

  static constexpr uint32_t kVertexBinding = 0u;
  std::vector<Vertex> vertices_ = {{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                   {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                                   {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                                   {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}};

  std::vector<uint32_t> indices_ = {0, 1, 2, 2, 3, 0};

  logi::RenderPass renderPass_;
  logi::ShaderModule vertexShader_;
  logi::ShaderModule fragmentShader_;

  logi::MemoryAllocator allocator_;
  logi::DescriptorPool descriptorPool_;

  logi::VMABuffer vertexBuffer_;
  logi::VMABuffer indexBuffer_;
  std::vector<logi::VMABuffer> matricesUBOBuffers_;

  Texture texture_;

  PipelineLayoutData pipelineLayoutData_;
  std::vector<logi::DescriptorSet> descriptorSets_;

  logi::Pipeline pipeline_;
  std::vector<logi::Framebuffer> framebuffers_;
};

#endif