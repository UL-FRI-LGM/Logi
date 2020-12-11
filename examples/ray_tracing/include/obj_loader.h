// Adapted from: https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR/blob/master/common/obj_loader.h

#pragma once
#include "../../base/include/tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <array>
#include <iostream>
#include <unordered_map>
#include <vector>

// Structure holding the material
struct MaterialObj
{
  glm::vec3 ambient       = glm::vec3(0.1f, 0.1f, 0.1f);
  glm::vec3 diffuse       = glm::vec3(0.7f, 0.7f, 0.7f);
  glm::vec3 specular      = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 transmittance = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 emission      = glm::vec3(0.0f, 0.0f, 0.10);
  float         shininess     = 0.f;
  float         ior           = 1.0f;  // index of refraction
  float         dissolve      = 1.f;   // 1 == opaque; 0 == fully transparent
      // illumination model (see http://www.fileformat.info/format/material/)
  int illum     = 0;
  int textureID = -1;
};

// OBJ representation of a vertex
struct VertexObj
{
  glm::vec3 pos;
  glm::vec3 nrm;
  glm::vec3 color;
  glm::vec2 texCoord;
};

struct shapeObj
{
  uint32_t offset;
  uint32_t nbIndex;
  uint32_t matIndex;
};

class ObjLoader
{
public:
  void loadModel(const std::string& path);

  std::vector<VertexObj>   m_vertices;
  std::vector<uint32_t>    m_indices;
  std::vector<MaterialObj> m_materials;
  std::vector<std::string> m_textures;
  std::vector<uint32_t>    m_matIndx;
};