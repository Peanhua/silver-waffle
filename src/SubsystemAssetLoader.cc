#include "SubsystemAssetLoader.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include <cassert>
#include <json11.hpp>
#include <iostream>
#include <fstream>


SubsystemAssetLoader * AssetLoader = nullptr;


SubsystemAssetLoader::SubsystemAssetLoader()
  : Subsystem("AssetLoader")
{
}


bool SubsystemAssetLoader::Start()
{
  assert(!AssetLoader);
  AssetLoader = this;

  return true;
}


void SubsystemAssetLoader::Stop()
{
  if(AssetLoader == this)
    AssetLoader = nullptr;
}


const std::string & SubsystemAssetLoader::LoadText(const std::string & filename)
{
  auto it = _text_assets.find(filename);
  if(it != _text_assets.end())
    return (*it).second;

  std::ifstream fp(filename);
  if(fp)
    {
      std::string text;
      std::string tmp;
      while(std::getline(fp, tmp))
        text += tmp + '\n';

      if(fp.eof())
        {
          _text_assets[filename] = text;
          return _text_assets[filename];
        }
      else
        std::cerr << "Failed to read '" << filename << "'" << std::endl;
    }

  static std::string nothing;
  return nothing;
}


ShaderProgram * SubsystemAssetLoader::LoadShaderProgram(const std::string & name)
{
  auto it = _shader_programs.find(name);
  if(it != _shader_programs.end())
    return (*it).second;

  auto vs = LoadText("Shaders/" + name + ".vert");
  auto fs = LoadText("Shaders/" + name + ".frag");
  auto gs = LoadText("Shaders/" + name + ".geom");
  std::vector<std::string> vss({vs});
  std::vector<std::string> fss({fs});
  std::vector<std::string> gss({gs});
  auto sp = new ShaderProgram(vss, fss, gss);
  assert(sp);
  _shader_programs[name] = sp;
  return sp;
}


Mesh * SubsystemAssetLoader::LoadMesh(const std::string & name)
{
  auto it = _meshes.find(name);
  if(it != _meshes.end())
    return (*it).second;

  auto mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT | Mesh::OPTION_NORMAL);
  assert(mesh);
  if(mesh->LoadFromFile("3d-models/" + name + ".dae"))
    {
      mesh->UpdateGPU();
      mesh->CalculateBoundingSphereRadius();
      _meshes[name] = mesh;
    }

  const std::string filename("3d-models/" + name + ".json");
  std::string json_string = LoadText(filename);
  if(json_string.size() > 0)
    {
      std::string err;
      auto config = json11::Json::parse(json_string, err);
      if(config.is_object())
        {
          if(config["rotate-x"].is_number())
            {
              auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(config["rotate-x"].number_value())), glm::vec3(1, 0, 0));
              mesh->ApplyTransform(transform);
            }
          if(config["rotate-y"].is_number())
            {
              auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(config["rotate-y"].number_value())), glm::vec3(0, 1, 0));
              mesh->ApplyTransform(transform);
            }
          if(config["rotate-z"].is_number())
            {
              auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(config["rotate-z"].number_value())), glm::vec3(0, 0, 1));
              mesh->ApplyTransform(transform);
            }
        }
      else
        std::cerr << "Error while parsing '" << filename << "': " << err << std::endl;
    }
  
  return mesh;
}


Image * SubsystemAssetLoader::LoadImage(const std::string & name)
{
  auto it = _images.find(name);
  if(it != _images.end())
    return (*it).second;

  auto rv = new Image(true);
  if(rv->Load(std::string("Images/") + name + ".png"))
    {
      rv->UpdateGPU(true, true);
      _images[name] = rv;
    }

  assert(rv);
  return rv;
}
