#ifndef EXAMPLE_VULKAN_TUTORIAL_PORT_H
#define EXAMPLE_VULKAN_TUTORIAL_PORT_H

#include "base/utility.h"
#include "base/example_base.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

struct StagingInfo {
    logi::VMABuffer* stagingBuffer;
    logi::VMABuffer* dstBuffer;
    vk::BufferUsageFlags dstUsage;
    vk::DeviceSize bufferSize;
};

struct Texture {
  logi::VMAImage image;
  logi::ImageView imageView;
  logi::Sampler sampler;
};

struct ImageResource {
    logi::Image image;
    logi::ImageView imageView;
    vk::Format format;
};

class VulkanTutorialPort : public ExampleBase 
{
    protected:
     void loadShaders();

     void loadModel();

     void allocateBuffers();

     void updateUniformBuffers();

     void createDepthResource();

     void createTexture();

     void initializeDescriptorSets();

     void createRenderPass();

     void createGraphicsPipeline();

     void createFramebuffers();

     void recordCommandBuffers();

     void initialize() override;
    
     void onViewChanged() override;

     void onSwapChainRecreate() override;

     void draw() override;

    private:
     const std::string TEXTURE_PATH = "./build/examples/vulkanTutorialPort/resources/images/viking_room.png";
     const std::string MODEL_PATH = "./build/examples/vulkanTutorialPort/resources/models/viking_room.obj";

     struct {
         glm::mat4 modelView = glm::mat4(1);
         glm::mat4 projection = glm::mat4(1);
     } ubo_;

     static constexpr uint32_t vertBinding = 0;       
     utility::ModelObj model_;       

     logi::MemoryAllocator allocator_;
     logi::VMABuffer vertexBuffer_;
     logi::VMABuffer indexBuffer_;

     Texture texture_;
     ImageResource depthResource_;
     std::vector<logi::VMABuffer> matrixUniformBuffers_;

     utility::ShaderReflection shaderReflection_;
     utility::PipelineLayoutData pipelineLayoutData_;

     logi::DescriptorPool descriptorPool_;
     std::vector<logi::DescriptorSet> descriptorSets_;

     logi::RenderPass renderPass_;
     logi::Pipeline graphicsPipeline_;

     std::vector<logi::Framebuffer> framebuffers_;
};

#endif
