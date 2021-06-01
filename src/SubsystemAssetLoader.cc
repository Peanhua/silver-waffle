#include "SubsystemAssetLoader.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SolarSystemObject.hh"
#include <cassert>
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

json11::Json * SubsystemAssetLoader::LoadJson(const std::string & filename)
{
  auto it = _jsons.find(filename);
  if(it != _jsons.end())
    return (*it).second;

  auto json = new json11::Json();
  std::string json_string = LoadText(filename + ".json");
  if(json_string.size() > 0)
    {
      std::string err;
      *json = json11::Json::parse(json_string, err);
      if(!json->is_object())
        {
          std::cerr << "Error while parsing '" << filename << ".json': " << err << std::endl;
          delete json;
          json = nullptr;
        }
    }
  _jsons[filename] = json;

  return json;
}


Mesh * SubsystemAssetLoader::LoadMesh(const std::string & name)
{
  auto it = _meshes.find(name);
  if(it != _meshes.end())
    return (*it).second;

  auto config = LoadJson("3d-models/" + name);
  
  auto mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT | Mesh::OPTION_NORMAL);
  assert(mesh);
  
  if(mesh->LoadFromFile("3d-models/" + name + ".dae"))
    {
      mesh->UpdateGPU();
      mesh->CalculateBoundingSphereRadius();
      _meshes[name] = mesh;
    }

  if(config)
    {
      auto rotate = [&mesh, &config](const std::string & rotname, const glm::vec3 & axis)
      {
        if((*config)[rotname].is_number())
          {
            auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians((*config)[rotname].number_value())), axis);
            mesh->ApplyTransform(transform);
          }
      };
      rotate("rotate-x", glm::vec3(1, 0, 0));
      rotate("rotate-y", glm::vec3(0, 1, 0));
      rotate("rotate-z", glm::vec3(0, 0, 1));
    }
  
  return mesh;
}


Image * SubsystemAssetLoader::LoadImage(const std::string & name)
{
  auto it = _images.find(name);
  if(it != _images.end())
    return (*it).second;

  auto rv = new Image(true);

  auto start = name.find_last_of('/');
  if(start == name.npos)
    start = 0;
  else
    if(start > 0)
      start++;

  auto end = name.size();
  auto dotpos = name.find_last_of('.');
  if(dotpos != name.npos && dotpos > start)
    end = dotpos - start;

  std::string stripped_name = name.substr(start, end);
  std::string quality = "";
  if(true)
    quality = "-low";

  if(rv->Load(std::string("Images/") + stripped_name + quality + ".png") ||
     rv->Load(std::string("Images/") + stripped_name + quality + ".jpg") ||
     rv->Load(std::string("Images/") + stripped_name +           ".png") ||
     rv->Load(std::string("Images/") + stripped_name +           ".jpg"))
    {
      rv->UpdateGPU(true, true);
      _images[name] = rv;
    }
  else
    {
      delete rv;
      rv = nullptr;
    }
  
  assert(rv);
  return rv;
}


SolarSystemObject * SubsystemAssetLoader::LoadSolarSystemObject(int type, unsigned int index)
{
  auto it = _solar_system_objects.find(type);
  if(it == _solar_system_objects.end())
    {
      auto config = LoadJson("Data/SolarSystemObjects");
      assert(config);

      std::string arrname = "unknown";
      auto typ = static_cast<SolarSystemObject::Type>(type);
      switch(typ)
        {
        case SolarSystemObject::TYPE_STAR:   arrname = "stars";   break;
        case SolarSystemObject::TYPE_PLANET: arrname = "planets"; break;
        }
      assert((*config)[arrname].is_array());
      
      auto objs = new std::vector<SolarSystemObject *>();
      for(auto data : (*config)[arrname].array_items())
        {
          glm::vec2 ring(0, 0);
          if(data["ring"].is_array())
            {
              auto a = data["ring"].array_items();
              assert(a.size() == 2);
              ring.x = static_cast<float>(a[0].number_value());
              ring.y = static_cast<float>(a[1].number_value());
            }
          
          auto obj = new SolarSystemObject(typ,
                                           data["texture"].string_value(),
                                           data["radius"].number_value(),
                                           ring);
          objs->push_back(obj);
        }
      _solar_system_objects[type] = objs;

      it = _solar_system_objects.find(type);
      if(it == _solar_system_objects.end())
        return nullptr;
    }

  auto objs = (*it).second;
  assert(objs);
  if(index >= objs->size())
    return nullptr;

  return objs->at(index);
}

