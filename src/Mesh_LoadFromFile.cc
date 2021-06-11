#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

static glm::mat4 ToGLM(const aiMatrix4x4 & from);


bool Mesh::LoadFromFile(const std::string & filename, const std::string & shader_prefix)
{
  Assimp::Importer importer;

  auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if(!scene)
    {
      std::cerr << "Failed to load " << filename << ": " << importer.GetErrorString() << std::endl;
      return false;
    }

  return LoadFromAssimpNode(scene, scene->mRootNode, true, shader_prefix);
}


bool Mesh::LoadFromAssimpNode(const aiScene * scene, aiNode * node, bool first, const std::string & shader_prefix)
{
  /* The first empty node contains rotation matrix rotating the model to +y = up,
  ** so the first node can be skipped.
  */
  if(first && node->mNumMeshes == 0 && node->mNumChildren == 1)
    return LoadFromAssimpNode(scene, node->mChildren[0], false, shader_prefix);
  
  if(!first)
    _transform = ToGLM(node->mTransformation);
 
  
  bool success = true;

  _name = node->mName.C_Str();
 
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

      if(material->GetTextureCount(aiTextureType_DIFFUSE) == 1)
        {
          assert(mesh->GetNumUVChannels() == 1);
          assert(mesh->mNumUVComponents[0] == 2);
          assert(mesh->HasTextureCoords(0));
          _options |= OPTION_TEXTURE;
          
          aiString path;
          auto x = material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
          assert(x == aiReturn_SUCCESS);
          auto texture = AssetLoader->LoadImage(path.C_Str());
          assert(texture);
          SetTexture(0, texture);
        }
      
      for(unsigned int vi = 0; vi < mesh->mNumVertices; vi++)
        {
          AddVertex(glm::vec3(mesh->mVertices[vi].x,
                              mesh->mVertices[vi].y,
                              mesh->mVertices[vi].z));
                    
          if(_options & OPTION_BLEND)
            AddColor(diffuse_color);
          else
            AddColor(diffuse_color.xyz());
          
          AddNormal(glm::vec3(mesh->mNormals[vi].x,
                              mesh->mNormals[vi].y,
                              mesh->mNormals[vi].z));

          if(_options & OPTION_TEXTURE)
            {
              assert(mesh->GetNumUVChannels() == 1);
              assert(mesh->mNumUVComponents[0] == 2);
              assert(mesh->HasTextureCoords(0));
              AddTexCoord(glm::vec2(mesh->mTextureCoords[0][vi].x,
                                    mesh->mTextureCoords[0][vi].y));
            }
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

  if(_options & OPTION_TEXTURE)
    SetShaderProgram(AssetLoader->LoadShaderProgram(shader_prefix + "-Texture"));
  else
    SetShaderProgram(AssetLoader->LoadShaderProgram(shader_prefix + "-Color"));

  
  for(unsigned int i = 0; success && i < node->mNumChildren; i++)
    {
      auto childnode = node->mChildren[i];
      if(childnode->mNumMeshes > 0 || childnode->mNumChildren > 0)
        {
          auto child = new Mesh(_options);
          assert(child);
          success = child->LoadFromAssimpNode(scene, childnode, false, shader_prefix);
          assert(success);
          _children.push_back(child);
        }
    }
  assert(success);

  return success;
}


static glm::mat4 ToGLM(const aiMatrix4x4 & from)
{
  glm::mat4 dst;

  dst[0][0] = from.a1;
  dst[0][1] = from.a2;
  dst[0][2] = from.a3;
  dst[0][3] = from.a4;

  dst[1][0] = from.b1;
  dst[1][1] = from.b2;
  dst[1][2] = from.b3;
  dst[1][3] = from.b4;

  dst[2][0] = from.c1;
  dst[2][1] = from.c2;
  dst[2][2] = from.c3;
  dst[2][3] = from.c4;

  dst[3][0] = from.d1;
  dst[3][1] = from.d2;
  dst[3][2] = from.d3;
  dst[3][3] = from.d4;
  
  return dst;
}
