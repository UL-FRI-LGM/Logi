// Adapted from: https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR/blob/master/common/obj_loader.cpp

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader.h"

void ObjLoader::loadModel(const std::string& path)
{
  tinyobj::ObjReader reader;
  reader.ParseFromFile(path);
  if(!reader.Valid())
  {
    std::cerr << "Cannot load: " << path << std::endl;
    assert(reader.Valid());
  }

  // Collecting the material in the scene
  for(const auto& material : reader.GetMaterials())
  {
    MaterialObj m;
    m.ambient  = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
    m.diffuse  = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
    m.specular = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
    m.emission = glm::vec3(material.emission[0], material.emission[1], material.emission[2]);
    m.transmittance = glm::vec3(material.transmittance[0], material.transmittance[1],
                                    material.transmittance[2]);
    m.dissolve      = material.dissolve;
    m.ior           = material.ior;
    m.shininess     = material.shininess;
    m.illum         = material.illum;
    if(!material.diffuse_texname.empty())
    {
      m_textures.push_back(material.diffuse_texname);
      m.textureID = static_cast<int>(m_textures.size()) - 1;
    }

    m_materials.emplace_back(m);
  }

  // If there were none, add a default
  if(m_materials.empty())
    m_materials.emplace_back(MaterialObj());

  const tinyobj::attrib_t& attrib = reader.GetAttrib();

  for(const auto& shape : reader.GetShapes())
  {
    m_vertices.reserve(shape.mesh.indices.size() + m_vertices.size());
    m_indices.reserve(shape.mesh.indices.size() + m_indices.size());
    m_matIndx.insert(m_matIndx.end(), shape.mesh.material_ids.begin(),
                     shape.mesh.material_ids.end());

    for(const auto& index : shape.mesh.indices)
    {
      VertexObj    vertex = {};
      const float* vp     = &attrib.vertices[3 * index.vertex_index];
      vertex.pos          = {*(vp + 0), *(vp + 1), *(vp + 2)};

      if(!attrib.normals.empty() && index.normal_index >= 0)
      {
        const float* np = &attrib.normals[3 * index.normal_index];
        vertex.nrm      = {*(np + 0), *(np + 1), *(np + 2)};
      }

      if(!attrib.texcoords.empty() && index.texcoord_index >= 0)
      {
        const float* tp = &attrib.texcoords[2 * index.texcoord_index + 0];
        vertex.texCoord = {*tp, 1.0f - *(tp + 1)};
      }

      if(!attrib.colors.empty())
      {
        const float* vc = &attrib.colors[3 * index.vertex_index];
        vertex.color    = {*(vc + 0), *(vc + 1), *(vc + 2)};
      }

      m_vertices.push_back(vertex);
      m_indices.push_back(static_cast<int>(m_indices.size()));
    }
  }

  // Fixing material indices
  for(auto& mi : m_matIndx)
  {
    if(mi < 0 || mi > m_materials.size())
      mi = 0;
  }


  // Compute normal when no normal were provided.
  if(attrib.normals.empty())
  {
    for(size_t i = 0; i < m_indices.size(); i += 3)
    {
      VertexObj& v0 = m_vertices[m_indices[i + 0]];
      VertexObj& v1 = m_vertices[m_indices[i + 1]];
      VertexObj& v2 = m_vertices[m_indices[i + 2]];

      glm::vec3 n = glm::normalize(glm::cross((v1.pos - v0.pos), (v2.pos - v0.pos)));
      v0.nrm          = n;
      v1.nrm          = n;
      v2.nrm          = n;
    }
  }
}