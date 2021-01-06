#include "ray_tracing.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

RayTracing::RayTracing(const ExampleConfiguration& config) : ExampleBase(config) {}

void RayTracing::initRayTracing() {
  // Query ray tracing properties of physical device
  auto properties = vulkanState_.physicalDevice_.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceRayTracingPropertiesNV>();
  rayTracingProperties_ = properties.get<vk::PhysicalDeviceRayTracingPropertiesNV>();
}

void RayTracing::loadShaders() {
  shaderReflection_ = utility::loadShaders(vulkanState_,
                                           "./shaders/triangle.vert.spv",
                                           "./shaders/triangle.frag.spv");

  pipelineLayoutData_ = utility::createPipelineLayout(vulkanState_, shaderReflection_);
}

// Loads model using obj_loader and allocates buffers on device
void RayTracing::loadModel(const std::string& path, glm::mat4 transform) {

  ObjLoader loader;
  loader.loadModel(path);


  // Convert from srgb to linear
  for(auto m : loader.m_materials) {
    float exp = 2.2f;
    m.ambient = glm::vec3(glm::pow(m.ambient.x, 2.2f), glm::pow(m.ambient.y, 2.2f), glm::pow(m.ambient.z, 2.2f));
    m.diffuse = glm::vec3(glm::pow(m.diffuse.x, 2.2f), glm::pow(m.diffuse.y, 2.2f), glm::pow(m.diffuse.z, 2.2f));
    m.specular = glm::vec3(glm::pow(m.specular.x, 2.2f), glm::pow(m.specular.y, 2.2f), glm::pow(m.specular.z, 2.2f));
  }


  ObjInstance instance;
  instance.objIndex = static_cast<uint32_t>(objInstances_.size());
  instance.transform = transform;
  instance.transformIT = glm::transpose(glm::inverse(transform));
  instance.txtOffset = static_cast<uint32_t>(textures_.size());

  ObjModel model;
  model.nbVertices = static_cast<uint32_t>(loader.m_vertices.size());
  model.nbIndices = static_cast<uint32_t>(loader.m_indices.size());


  // Allocate buffers
  utility::BufferAllocateInfo vertexInfo;
  vertexInfo.data = loader.m_vertices.data();
  vertexInfo.size = loader.m_vertices.size() * sizeof(VertexObj);
  vertexInfo.sharingMode = vk::SharingMode::eExclusive;
  // NOTE: update for KHR
  vertexInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer |
                     vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eRayTracingNV;

  utility::BufferAllocateInfo indexInfo;
  indexInfo.data = loader.m_indices.data();
  indexInfo.size = loader.m_indices.size() * sizeof(uint32_t);
  indexInfo.sharingMode = vk::SharingMode::eExclusive;
  // NOTE: update for KHR
  indexInfo.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eStorageBuffer |
                     vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eRayTracingNV;

  utility::BufferAllocateInfo matInfo;
  matInfo.data = loader.m_materials.data();
  matInfo.size = loader.m_materials.size() * sizeof(MaterialObj);
  matInfo.sharingMode = vk::SharingMode::eExclusive;
  matInfo.usage = vk::BufferUsageFlagBits::eStorageBuffer;

  utility::BufferAllocateInfo matIndInfo;
  matIndInfo.data = loader.m_matIndx.data();
  matIndInfo.size = loader.m_matIndx.size() * sizeof(uint32_t);
  matIndInfo.sharingMode = vk::SharingMode::eExclusive;
  matIndInfo.usage = vk::BufferUsageFlagBits::eStorageBuffer;

  std::vector<utility::BufferAllocateInfo> bufferAllocInfos = {vertexInfo, indexInfo, matInfo, matIndInfo};
  std::vector<logi::VMABuffer> buffers;

  utility::allocateBufferStaged(vulkanState_, VMA_MEMORY_USAGE_GPU_ONLY, bufferAllocInfos, buffers);
  model.vertexBuffer = buffers[0];
  model.indexBuffer = buffers[1];
  model.matColorBuffer = buffers[2];
  model.matIndexBuffer = buffers[3];


  // Allocate textures
  allocateTextures(loader.m_textures);


  objInstances_.push_back(instance);
  objModels_.push_back(model);
}

// NOTE: extremely inefficient! - each allocation is separate, better to batch allocations
void RayTracing::allocateTextures(const std::vector<std::string>& texturePaths) {

  vk::SamplerCreateInfo samplerInfo;
  samplerInfo.magFilter = vk::Filter::eLinear;
  samplerInfo.minFilter = vk::Filter::eLinear;
  samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;

  logi::Sampler sampler = vulkanState_.defaultLogicalDevice_->createSampler(samplerInfo);

  // Create dummy texture if no texture is present
  if(texturePaths.empty() && textures_.empty()) {
    
    // Dummy
    std::array<uint8_t, 4> color{255u, 255u, 255u, 255u};
    vk::DeviceSize bufferSize = sizeof(color);
    uint32_t texWidth = 1;
    uint32_t texHeight = 1;
    auto imageSize = vk::Extent2D(1, 1);


    // Staging buffer
    VmaAllocationCreateInfo allocStaged = {};
    allocStaged.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_ONLY;   

    vk::BufferCreateInfo imageStaging;
    imageStaging.size = bufferSize;
    imageStaging.usage = vk::BufferUsageFlagBits::eTransferSrc;
    imageStaging.sharingMode = vk::SharingMode::eExclusive;

    logi::VMABuffer stagingBuffer = vulkanState_.defaultAllocator_->createBuffer(imageStaging, allocStaged);
    stagingBuffer.writeToBuffer(&color, bufferSize); 

    // Image allocation
    Texture texture;

    texture.image = utility::createImage(vulkanState_, texWidth, texHeight, vk::Format::eR8G8B8A8Unorm, 
                        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, VMA_MEMORY_USAGE_GPU_ONLY);

    utility::transitionImageLayout(vulkanState_, texture.image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

    utility::copyBufferToImage(vulkanState_, stagingBuffer, texture.image, texWidth, texHeight);

    utility::transitionImageLayout(vulkanState_, texture.image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

    // Image View creation
    texture.imageView = texture.image.createImageView({}, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, {}, 
                                                      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    texture.sampler = sampler;

    textures_.push_back(texture);
  } else {
    for(auto path : texturePaths) {
      path = "../resources/textures/" + path;

      Texture texture;
      texture.image = utility::loadImageStaged(vulkanState_, VMA_MEMORY_USAGE_GPU_ONLY, vk::ImageUsageFlagBits::eSampled,
                                                vk::ImageLayout::eShaderReadOnlyOptimal, path.c_str());
      texture.imageView = texture.image.createImageView({}, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, {}, 
                                                        vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
      texture.sampler = sampler;

      textures_.push_back(texture);
    }
  }
}

void RayTracing::allocateBuffers() {
  // Create vertex buffer
  utility::BufferAllocateInfo vertexBufferAllocateInfo = {};
  vertexBufferAllocateInfo.data = vertices.data();
  vertexBufferAllocateInfo.size = vertices.size() * sizeof(Vertex);
  vertexBufferAllocateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
  vertexBufferAllocateInfo.sharingMode = vk::SharingMode::eExclusive;

  std::vector<utility::BufferAllocateInfo> bufferAllocateInfo = {vertexBufferAllocateInfo};
  std::vector<logi::VMABuffer> buffers;

  utility::allocateBuffer(vulkanState_, VMA_MEMORY_USAGE_CPU_TO_GPU, bufferAllocateInfo, buffers);
  vertexBuffer_ = buffers[0];

  // Create and init matrices UBO buffer
  VmaAllocationCreateInfo allocationInfo = {};
  allocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;

  vk::BufferCreateInfo matricesBufferInfo;
  matricesBufferInfo.size = sizeof(uboMatrices);
  matricesBufferInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst;
  matricesBufferInfo.sharingMode = vk::SharingMode::eExclusive;

  for (size_t i = 0; i < swapchainImages_.size(); i++) {
    matricesUBOBuffers_.emplace_back(vulkanState_.defaultAllocator_->createBuffer(matricesBufferInfo, allocationInfo));
  }

  updateMatrixBuffers();
}

void RayTracing::updateMatrixBuffers() {
  // Update matrices
  uboMatrices.projectionMatrix = glm::perspective(
    glm::radians(60.0f), (float) swapchainImageExtent_.width / (float) swapchainImageExtent_.height, 0.1f, 256.0f);

  uboMatrices.viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPos.x, cameraPos.y, zoom));

  uboMatrices.modelMatrix = glm::mat4(1.0f);
  uboMatrices.modelMatrix = glm::rotate(uboMatrices.modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  uboMatrices.modelMatrix = glm::rotate(uboMatrices.modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  uboMatrices.modelMatrix = glm::rotate(uboMatrices.modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  for (const auto& buffer : matricesUBOBuffers_) {
    buffer.writeToBuffer(&uboMatrices, sizeof(uboMatrices));
  }
}

void RayTracing::initializeDescriptorSets() {
  // Create descriptor pool.
  vk::DescriptorPoolSize poolSize;
  poolSize.type = vk::DescriptorType::eUniformBuffer;
  poolSize.descriptorCount = static_cast<uint32_t>(swapchainImages_.size());

  vk::DescriptorPoolCreateInfo poolInfo;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.poolSizeCount = 1u;
  poolInfo.maxSets = static_cast<uint32_t>(swapchainImages_.size());

  descriptorPool_ = vulkanState_.defaultLogicalDevice_->createDescriptorPool(poolInfo);

  // Create descriptor sets.
  std::vector<vk::DescriptorSetLayout> layouts(swapchainImages_.size(), pipelineLayoutData_.descriptorSetLayouts[0]);
  descriptorSets_ = descriptorPool_.allocateDescriptorSets(layouts);

  for (size_t i = 0; i < descriptorSets_.size(); i++) {
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = matricesUBOBuffers_[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(uboMatrices);

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = descriptorSets_[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vulkanState_.defaultLogicalDevice_->updateDescriptorSets(descriptorWrite);
  }
}

void RayTracing::createBottomLevelAS() {
  // Object to vkGeometry
  std::vector<std::vector<vk::GeometryNV>> geoms; // In our case only one geometry in each BLAS
  geoms.reserve(objModels_.size());

  for(const auto& obj : objModels_) {
    auto geo = objectToVkGeometryNV(obj);
    geoms.push_back({geo});
  }

  // Create BLAS-es
  std::vector<vk::AccelerationStructureInfoNV> acInfos;
  acInfos.reserve(geoms.size());
  vk::MemoryRequirements memoryRequirements;

  vk::DeviceSize maxScratchSize{0};

  for(int i = 0; i < geoms.size(); i++) {
    vk::AccelerationStructureInfoNV acInfo;
    acInfo.type = vk::AccelerationStructureTypeNV::eBottomLevel;
    acInfo.flags = vk::BuildAccelerationStructureFlagBitsNV::ePreferFastTrace;
    acInfo.instanceCount = 0;
    acInfo.geometryCount = static_cast<uint32_t>(geoms[i].size());
    acInfo.pGeometries = geoms[i].data();

    acInfos.push_back(acInfo);

    vk::AccelerationStructureCreateInfoNV asCreateInfo;
    asCreateInfo.compactedSize = 0;
    asCreateInfo.info = acInfo;

    VmaAllocationCreateInfo asAllocationInfo = {};
    asAllocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;

    // Create acceleration structure
    logi::VMAAccelerationStructureNV accelerationStructure =
        vulkanState_.defaultAllocator_->createAccelerationStructureNV(asCreateInfo, asAllocationInfo);
    blas_.push_back(accelerationStructure);

    // Query scratch buffer size
    memoryRequirements =
       accelerationStructure.getMemoryRequirementsNV(vk::AccelerationStructureMemoryRequirementsTypeNV::eBuildScratch).memoryRequirements;

    maxScratchSize = std::max(maxScratchSize, memoryRequirements.size);
  }


  // Allocate scratch buffer
  VmaAllocationCreateInfo scratchBufferAllocationInfo = {};
  scratchBufferAllocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
  scratchBufferAllocationInfo.memoryTypeBits = memoryRequirements.memoryTypeBits;

  vk::BufferCreateInfo scratchBufferInfo = {};
  scratchBufferInfo.size =  maxScratchSize;
  scratchBufferInfo.usage = vk::BufferUsageFlagBits::eRayTracingNV;
  scratchBufferInfo.sharingMode = vk::SharingMode::eExclusive;

  logi::VMABuffer scratchBuffer = vulkanState_.defaultAllocator_->createBuffer(scratchBufferInfo, scratchBufferAllocationInfo);


  // Build BLAS
  logi::CommandBuffer cmd = utility::beginSingleTimeCommand(vulkanState_, utility::Graphics);
  
  for(int i = 0; i < geoms.size(); i++) {
    cmd.buildAccelerationStructureNV(acInfos[i], nullptr, 0, false, blas_[i], nullptr, scratchBuffer, 0);

    vk::MemoryBarrier memoryBarrier;
    memoryBarrier.srcAccessMask = vk::AccessFlagBits::eAccelerationStructureWriteNV | vk::AccessFlagBits::eAccelerationStructureReadNV;
    memoryBarrier.dstAccessMask = vk::AccessFlagBits::eAccelerationStructureReadNV | vk::AccessFlagBits::eAccelerationStructureWriteNV;

    cmd.pipelineBarrier(vk::PipelineStageFlagBits::eAccelerationStructureBuildNV,
                        vk::PipelineStageFlagBits::eAccelerationStructureBuildNV, {}, memoryBarrier, {}, {});
  }

  utility::endSingleTimeCommand(vulkanState_, utility::Graphics, cmd);


  cmd.destroy();
  scratchBuffer.destroy();  
}

vk::GeometryNV RayTracing::objectToVkGeometryNV(const ObjModel& model) {
  vk::GeometryTrianglesNV triangles;

  triangles.setVertexData(model.vertexBuffer);
  triangles.setVertexOffset(0); 
  triangles.setVertexCount(model.nbVertices);
  triangles.setVertexStride(sizeof(VertexObj));
  triangles.setVertexFormat(vk::Format::eR32G32B32Sfloat);  // 3xfloat32 for vertices

  triangles.setIndexData(model.indexBuffer);
  triangles.setIndexOffset(0);
  triangles.setIndexCount(model.nbIndices);
  triangles.setIndexType(vk::IndexType::eUint32);  // 32-bit indices

  vk::GeometryDataNV geoData;
  geoData.setTriangles(triangles);

  vk::GeometryNV geometry;
  geometry.setGeometry(geoData);
  geometry.setFlags(vk::GeometryFlagBitsNV::eOpaque); // only opaque triangles

  return geometry;
}

void RayTracing::createTopLevelAS() {
  // Instance to ASInstanceNV
  std::vector<vk::AccelerationStructureInstanceNV> geometryInstances;
  geometryInstances.reserve(objInstances_.size());

  for(int i = 0; i < objInstances_.size(); i++) {
    uint64_t blasAddress;
    blas_[i].getHandleNV<uint64_t>(blasAddress);

    vk::AccelerationStructureInstanceNV vkInstanceNV = 
      instanceToVkASGeometryInstance(objInstances_[i], blasAddress);
    geometryInstances.push_back(vkInstanceNV);
  }

  // Create acceleration structure
  vk::AccelerationStructureInfoNV acInfo;
  acInfo.type = vk::AccelerationStructureTypeNV::eTopLevel;
  acInfo.flags = vk::BuildAccelerationStructureFlagBitsNV::ePreferFastTrace;
  acInfo.instanceCount = static_cast<uint32_t>(geometryInstances.size());

  vk::AccelerationStructureCreateInfoNV asCreateInfo;
  asCreateInfo.compactedSize = 0;
  asCreateInfo.info = acInfo;

  VmaAllocationCreateInfo asAllocationInfo = {};
  asAllocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;

  tlas_ = vulkanState_.defaultAllocator_->createAccelerationStructureNV(asCreateInfo, asAllocationInfo);

  // Query scratch buffer size
  vk::MemoryRequirements memoryRequirements =
      tlas_.getMemoryRequirementsNV(vk::AccelerationStructureMemoryRequirementsTypeNV::eBuildScratch).memoryRequirements;
  
  vk::DeviceSize scratchSize = memoryRequirements.size;

  // Allocate scratch buffer
  VmaAllocationCreateInfo bufferAllocationInfo = {};
  bufferAllocationInfo.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
  bufferAllocationInfo.memoryTypeBits = memoryRequirements.memoryTypeBits;

  vk::BufferCreateInfo scratchBufferInfo = {};
  scratchBufferInfo.size =  scratchSize;
  scratchBufferInfo.usage = vk::BufferUsageFlagBits::eRayTracingNV;
  scratchBufferInfo.sharingMode = vk::SharingMode::eExclusive;

  logi::VMABuffer scratchBuffer = vulkanState_.defaultAllocator_->createBuffer(scratchBufferInfo, bufferAllocationInfo);

  // Allocate buffer holding instance data
  utility::BufferAllocateInfo instanceBufferAllocate = {};
  instanceBufferAllocate.data = geometryInstances.data();
  instanceBufferAllocate.size = geometryInstances.size() * sizeof(vk::AccelerationStructureInstanceNV);
  instanceBufferAllocate.usage = vk::BufferUsageFlagBits::eRayTracingNV;
  instanceBufferAllocate.sharingMode = vk::SharingMode::eExclusive;

  std::vector<utility::BufferAllocateInfo> bufferAllocateInfos = {instanceBufferAllocate};
  std::vector<logi::VMABuffer> buffers;

  utility::allocateBufferStaged(vulkanState_, VMA_MEMORY_USAGE_GPU_ONLY, bufferAllocateInfos, buffers);
  logi::VMABuffer instanceBuffer = buffers[0];


  // Build TLAS
  logi::CommandBuffer cmd = utility::beginSingleTimeCommand(vulkanState_, utility::Graphics);

  vk::MemoryBarrier memoryBarrier;
  memoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
  memoryBarrier.dstAccessMask = vk::AccessFlagBits::eAccelerationStructureWriteNV;

  cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                      vk::PipelineStageFlagBits::eAccelerationStructureBuildNV, {}, memoryBarrier, {}, {});

  cmd.buildAccelerationStructureNV(acInfo, instanceBuffer, 0, VK_FALSE, tlas_, nullptr, scratchBuffer, 0);
                     
  utility::endSingleTimeCommand(vulkanState_, utility::Graphics, cmd);

  // Clear resources
  scratchBuffer.destroy();
}

vk::AccelerationStructureInstanceNV 
  RayTracing::instanceToVkASGeometryInstance(const ObjInstance& instance, uint64_t& blasAddress){
    vk::AccelerationStructureInstanceNV vkInstanceNV;

    // Convert to corect transform matrix
    glm::mat4 transp = glm::transpose(instance.transform);
    memcpy(&vkInstanceNV.transform, &transp, sizeof(vkInstanceNV.transform));

    vkInstanceNV.instanceCustomIndex = instance.objIndex;
    vkInstanceNV.mask = 0xff;
    vkInstanceNV.instanceShaderBindingTableRecordOffset = 0;
    vkInstanceNV.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV;
    vkInstanceNV.accelerationStructureReference = blasAddress;

    return vkInstanceNV;
}

void RayTracing::createRenderPass() {
  vk::AttachmentDescription colorAttachment;
  colorAttachment.format = swapchainImageFormat_;
  colorAttachment.samples = vk::SampleCountFlagBits::e1;
  colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
  colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
  colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

  vk::AttachmentReference colorAttachmentRef;
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  vk::SubpassDependency dependency;
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.srcAccessMask = {};
  dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

  vk::RenderPassCreateInfo renderPassCreateInfo;
  renderPassCreateInfo.attachmentCount = 1;
  renderPassCreateInfo.pAttachments = &colorAttachment;
  renderPassCreateInfo.subpassCount = 1;
  renderPassCreateInfo.pSubpasses = &subpass;
  renderPassCreateInfo.dependencyCount = 1;
  renderPassCreateInfo.pDependencies = &dependency;

  renderPass_ = vulkanState_.defaultLogicalDevice_->createRenderPass(renderPassCreateInfo);
}

void RayTracing::createGraphicalPipeline() {
  // Destroy existing pipeline.
  if (pipeline_) {
    pipeline_.destroy();
  }

  // Pipeline
  vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
  vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
  vertShaderStageInfo.module = shaderReflection_.vertexShader;
  vertShaderStageInfo.pName = "main";

  vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
  fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
  fragShaderStageInfo.module = shaderReflection_.fragmentShader;
  fragShaderStageInfo.pName = "main";

  vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  std::vector<logi::VertexAttributeReflectionInfo> vtxAttributeInfo =
    shaderReflection_.vertexShader.getVertexAttributeReflectionInfo("main");

  std::vector<vk::VertexInputAttributeDescription> vtxAttributeDescriptions;
  vtxAttributeDescriptions.reserve(vtxAttributeInfo.size());

  uint32_t offset = 0u;
  for (const auto& entry : vtxAttributeInfo) {
    vtxAttributeDescriptions.emplace_back(entry.location, kVertexBinding, entry.format, offset);
    offset += entry.elementSize;
  }

  vk::VertexInputBindingDescription vtxBindingDescription(kVertexBinding, sizeof(Vertex), vk::VertexInputRate::eVertex);

  vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
  vertexInputInfo.pVertexBindingDescriptions = &vtxBindingDescription;
  vertexInputInfo.vertexBindingDescriptionCount = 1u;
  vertexInputInfo.pVertexAttributeDescriptions = vtxAttributeDescriptions.data();
  vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vtxAttributeDescriptions.size());

  vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
  inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  vk::Viewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) swapchainImageExtent_.width;
  viewport.height = (float) swapchainImageExtent_.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  vk::Rect2D scissor;
  scissor.extent = swapchainImageExtent_;

  vk::PipelineViewportStateCreateInfo viewportState;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  vk::PipelineRasterizationStateCreateInfo rasterizer;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = vk::PolygonMode::eFill;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = vk::CullModeFlagBits::eNone;
  rasterizer.frontFace = vk::FrontFace::eClockwise;
  rasterizer.depthBiasEnable = VK_FALSE;

  vk::PipelineMultisampleStateCreateInfo multisampling;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
  vk::SampleMask sampleMask(0xFFFFFF);
  multisampling.pSampleMask = &sampleMask;

  vk::PipelineColorBlendAttachmentState colorBlendAttachment;
  colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  colorBlendAttachment.blendEnable = VK_FALSE;

  vk::PipelineColorBlendStateCreateInfo colorBlending;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = vk::LogicOp::eCopy;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  vk::GraphicsPipelineCreateInfo pipelineInfo;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = pipelineLayoutData_.layout;
  pipelineInfo.renderPass = renderPass_;
  pipelineInfo.subpass = 0;

  pipeline_ = vulkanState_.defaultLogicalDevice_->createGraphicsPipeline(pipelineInfo);
}

void RayTracing::createFrameBuffers() {
  // Destroy previous framebuffers.
  for (const auto& framebuffer : framebuffers_) {
    framebuffer.destroy();
  }
  framebuffers_.clear();

  // Create new framebuffers.
  for (const auto& imageView : swapchainImageViews_) {
    vk::FramebufferCreateInfo createInfo;
    createInfo.renderPass = renderPass_;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &static_cast<const vk::ImageView&>(imageView);
    createInfo.width = swapchainImageExtent_.width;
    createInfo.height = swapchainImageExtent_.height;
    createInfo.layers = 1;

    framebuffers_.emplace_back(vulkanState_.defaultLogicalDevice_->createFramebuffer(createInfo));
  }
}

void RayTracing::recordCommandBuffers() {
  // Destroy old command buffers.
  for (const auto& cmdBuffer : primaryGraphicsCmdBuffers_) {
    cmdBuffer.reset();
  }

  for (size_t i = 0; i < primaryGraphicsCmdBuffers_.size(); i++) {
    vk::CommandBufferBeginInfo beginInfo = {};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

    primaryGraphicsCmdBuffers_[i].begin(beginInfo);

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = renderPass_;
    renderPassInfo.framebuffer = framebuffers_[i];
    renderPassInfo.renderArea.extent = swapchainImageExtent_;

    vk::ClearValue clearValue;
    clearValue.color.setFloat32({0.0, 0.0, 0.0, 1.0});
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    primaryGraphicsCmdBuffers_[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    primaryGraphicsCmdBuffers_[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_);

    primaryGraphicsCmdBuffers_[i].bindVertexBuffers(0, static_cast<vk::Buffer>(vertexBuffer_), static_cast<VkDeviceSize>(0ul));
    primaryGraphicsCmdBuffers_[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayoutData_.layout, 0,
                                                     static_cast<vk::DescriptorSet>(descriptorSets_[i]));

    primaryGraphicsCmdBuffers_[i].draw(3, 1, 0, 0);
    primaryGraphicsCmdBuffers_[i].endRenderPass();
    primaryGraphicsCmdBuffers_[i].end();
  }
}

void RayTracing::onViewChanged() {
  updateMatrixBuffers();
}

void RayTracing::onSwapChainRecreate() {
  createFrameBuffers();
  createGraphicalPipeline();
  recordCommandBuffers();
}

void RayTracing::initialize() {
  zoom = -2.5f;

  initRayTracing();

  loadShaders();

  // Set allocator
  logi::MemoryAllocator allocator = vulkanState_.defaultLogicalDevice_->createMemoryAllocator();
  vulkanState_.addAllocator("MainAlloc", allocator);
  vulkanState_.setDefaultAllocator("MainAlloc");

  loadModel(MODEL_PATH);
  createBottomLevelAS();
  createTopLevelAS();

  allocateBuffers();
  initializeDescriptorSets();

  createRenderPass();
  createFrameBuffers();
  createGraphicalPipeline();
  recordCommandBuffers();
}

void RayTracing::draw() {}
