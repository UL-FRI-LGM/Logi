#ifndef EXAMPLE_HELLO_TRIANGLE_H
#define EXAMPLE_HELLO_TRIANGLE_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include "example_base.h"
#include "imGUI_base.h"
#include "utility.h"

struct Vertex {
  glm::vec3 position;
  glm::vec2 uv;
};

class Fractals : public ImGUIBase {
 protected:
  void loadShaders();

  void allocateBuffers();

  void updateMatrixBuffers();

  void initializeDescriptorSets();

  void createRenderPass();

  void createGraphicalPipeline();

  void createFrameBuffers();

  void recordCommandBuffers();

  void imGUI_createUI() override;

  void onViewChanged() override;

  void onSwapChainRecreate() override;

  void initialize() override;

  void draw() override;

 private:
  struct {
    // Ray marching
    alignas(4) float minDist = 0.007f;

    // Camera
    alignas(16) glm::mat4 view = glm::mat4(1.0f);
    alignas(16) glm::vec4 cameraOrigin = glm::vec4(0.0f, 0.0f, 10.0f, 0.0f);

    alignas(4) float fov = 45.0f;
    alignas(4) float zoom = 0.0f;
    alignas(4) float aspectRatio = 0.0f;
    alignas(4) float viewportWidth = 0.0f;
    alignas(4) float viewportHeight = 0.0f;

    // Shading
    alignas(4) float dirLightShading = 1.0f;
    alignas(16) glm::vec3 dirLightColor = glm::vec3(233.0f, 184.0f, 25.0f) / 255.0f;
    alignas(4) float dirIntensity = 0.111f;
    alignas(16) glm::vec3 specularColor = glm::vec3(247.0f, 255.0f, 0.0f) / 255.0f;
    alignas(4) float specularIntensity = 0.990f;

    alignas(16) glm::vec3 backgroundColor = glm::vec3(13.0f, 5.0f, 73.0f) / 255.0f;
    alignas(4) float shadowSharpness = 6.0f;

    // Fractals
    alignas(4) int fractalType = 4;
    alignas(4) int iterations = 14;
    alignas(4) float fractalPower = 8.0;

    // Effects
    alignas(16) glm::vec3 glowColor = glm::vec3(0.0f, 211.0f, 248.0f) / 255.0f;
    alignas(4) float glowIntensity = 3.000f;
    alignas(16) glm::vec3 AOColor = glm::vec3(253.0f, 194.0f, 2.0f) / 255.0f;
    alignas(4) float AOIntensity = 0.100f;
    alignas(4) float fogCurve = 15.0f;
    alignas(4) float fogStartDist = 1.0f;
  } shaderSettings;

  int previousFractalType = 4;
  static constexpr uint32_t kVertexBinding = 0u;
  // Quad
  std::vector<Vertex> vertices = {{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
                                  {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
                                  {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                  {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
                                  {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                  {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}};

  logi::RenderPass renderPass_;

  utility::ShaderReflection shaderReflection_;
  utility::PipelineLayoutData pipelineLayoutData_;

  logi::VMABuffer vertexBuffer_;
  std::vector<logi::VMABuffer> matricesUBOBuffers_;
  std::vector<logi::VMABuffer> settingBuffers_;

  logi::DescriptorPool descriptorPool_;
  std::vector<logi::DescriptorSet> descriptorSets_;
  logi::Pipeline pipeline_;
  std::vector<logi::Framebuffer> framebuffers_;
};

#endif