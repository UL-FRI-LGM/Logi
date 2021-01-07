#ifndef EXAMPLE_HELLO_TRIANGLE_H
#define EXAMPLE_HELLO_TRIANGLE_H
#include <glm/glm.hpp>
#include "obj_loader.h"
#include "example_base.h"
#include "utility.h"

struct ShaderInfo 
{
  ShaderInfo(std::string path, std::string entryPoint);

  std::string path;
  std::string entryPoint;
};

struct PipelineLayoutDataRT 
{
  explicit PipelineLayoutDataRT(std::map<vk::ShaderStageFlagBits, logi::ShaderModule> shaders = {},
                              logi::PipelineLayout layout = {},
                              std::vector<logi::DescriptorSetLayout> descriptorSetLayouts = {});

  std::map<vk::ShaderStageFlagBits, logi::ShaderModule> shaders;
  logi::PipelineLayout layout;
  std::vector<logi::DescriptorSetLayout> descriptorSetLayouts;
};

// The OBJ model
struct ObjModel
{
  uint32_t     nbIndices{0};
  uint32_t     nbVertices{0};
  logi::VMABuffer  vertexBuffer;    // Device buffer of all 'Vertex'
  logi::VMABuffer  indexBuffer;     // Device buffer of the indices forming triangles
  logi::VMABuffer  matColorBuffer;  // Device buffer of array of 'Wavefront material'
  logi::VMABuffer  matIndexBuffer;  // Device buffer of array of 'Wavefront material'
};

// Instance of the OBJ
struct ObjInstance
{
  uint32_t  objIndex{0};     // Reference to the `m_objModel`
  uint32_t  txtOffset{0};    // Offset in `m_textures`
  glm::mat4 transform{1};    // Position of the instance
  glm::mat4 transformIT{1};  // Inverse transpose
};

struct Texture {
  logi::VMAImage image;
  logi::ImageView imageView;
  logi::Sampler sampler;
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
};

class RayTracing : public ExampleBase {
 public:
  RayTracing(const ExampleConfiguration& config = {});

 protected:
  void initRayTracing();

  void loadShaders();

  void loadModel(const std::string& path, glm::mat4 transform = glm::mat4(1.0f));

  void allocateTextures(const std::vector<std::string>& texturePaths);

  void allocateBuffers();

  void updateMatrixBuffers();

  void initializeDescriptorSets();

  void createBottomLevelAS();

  vk::GeometryNV objectToVkGeometryNV(const ObjModel& model);

  void createTopLevelAS();

  vk::AccelerationStructureInstanceNV 
    instanceToVkASGeometryInstance(const ObjInstance& instance, uint64_t& blasAddress);

  void createRenderPass();

  void createGraphicalPipeline();

  void createFrameBuffers();

  void recordCommandBuffers();

  void onViewChanged() override;

  void onSwapChainRecreate() override;

  void initialize() override;

  void draw() override;

 private:
  const std::string MODEL_PATH = "../resources/cube_multi.obj";
  // const std::string MODEL_PATH = "../resources/Medieval_building.obj";

  struct {
    glm::mat4 projectionMatrix = glm::mat4(1);
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::mat4 viewMatrix = glm::mat4(1);
  } uboMatrices;

  static constexpr uint32_t kVertexBinding = 0u;
  std::vector<Vertex> vertices = {{{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                                  {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                                  {{0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

  vk::PhysicalDeviceRayTracingPropertiesNV rayTracingProperties_;

  logi::RenderPass renderPass_;

  utility::ShaderReflection shaderReflection_;
  utility::PipelineLayoutData pipelineLayoutData_;

  PipelineLayoutDataRT pipelineLayoutDataRT_;

  std::vector<ObjInstance> objInstances_;
  std::vector<ObjModel> objModels_;
  std::vector<Texture> textures_;

  logi::VMAAccelerationStructureNV tlas_;
  std::vector<logi::VMAAccelerationStructureNV> blas_;

  logi::VMABuffer vertexBuffer_;
  std::vector<logi::VMABuffer> matricesUBOBuffers_;

  logi::DescriptorPool descriptorPool_;
  std::vector<logi::DescriptorSet> descriptorSets_;
  
  logi::Pipeline pipeline_;
  std::vector<logi::Framebuffer> framebuffers_;
};

#endif