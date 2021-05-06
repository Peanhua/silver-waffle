#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

static glm::mat4 ToGLM(const aiMatrix4x4 & from);


bool Mesh::LoadFromFile(const std::string & filename)
{
  Assimp::Importer importer;

  auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if(!scene)
    {
      std::cerr << "Failed to load " << filename << ": " << importer.GetErrorString() << std::endl;
      return false;
    }

  return LoadFromAssimpNode(scene, scene->mRootNode);
}


bool Mesh::LoadFromAssimpNode(const aiScene * scene, aiNode * node)
{
  bool success = true;
  
  _transform = ToGLM(node->mTransformation);
 
  SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
 
  for(unsigned int mi = 0; mi < node->mNumMeshes; mi++)
    {
      auto mesh = scene->mMeshes[node->mMeshes[mi]];
      auto startpos = _vertices.size() / 3;

      auto material = scene->mMaterials[mesh->mMaterialIndex];
      aiColor4D tmpcolor;
      glm::vec4 diffuse_color(1, 1, 1, 1);
      if(aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &tmpcolor) == AI_SUCCESS)
        {
          diffuse_color.r = tmpcolor.r;
          diffuse_color.g = tmpcolor.g;
          diffuse_color.b = tmpcolor.b;
          diffuse_color.a = tmpcolor.a;
        }
      
      for(unsigned int vi = 0; vi < mesh->mNumVertices; vi++)
        {
          glm::vec3 v(mesh->mVertices[vi].x,
                      mesh->mVertices[vi].y,
                      mesh->mVertices[vi].z);
          AddVertex(v);
          AddColor(diffuse_color);
        }

      for(unsigned int fi = 0; fi < mesh->mNumFaces; fi++)
        {
          auto face = &mesh->mFaces[fi];
          assert(face->mNumIndices == 3);

          assert(startpos < 0xffffffff);
          for(unsigned int ii = 0; ii < face->mNumIndices; ii++)
            AddElement(face->mIndices[ii] + static_cast<unsigned int>(startpos));
        }
    }

  for(unsigned int i = 0; success && i < node->mNumChildren; i++)
    {
      auto childnode = node->mChildren[i];
      if(childnode->mNumMeshes > 0 || childnode->mNumChildren > 0)
        {
          auto child = new Mesh(_options);
          assert(child);
          success = child->LoadFromAssimpNode(scene, childnode);
          assert(success);
          _children.push_back(child);
        }
    }
  assert(success);

  return success;
}


static glm::mat4 ToGLM(const aiMatrix4x4 & from)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  return glm::transpose(glm::make_mat4(&from.a1));
#pragma GCC diagnostic pop
}
