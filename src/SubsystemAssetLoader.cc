/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemAssetLoader.hh"
#include "CollisionShapeOBB.hh"
#include "CollisionShapeSphere.hh"
#include "Font.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "ShaderProgram.hh"
#include "SolarSystemObject.hh"
#include "SubsystemGfx.hh"
#include "SubsystemSettings.hh"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>


SubsystemAssetLoader * AssetLoader = nullptr;


SubsystemAssetLoader::SubsystemAssetLoader()
  : Subsystem("AssetLoader"),
    _texture_quality(0)
{
  std::string home; //(std::getenv("HOME"));
  if(!home.empty())
    _save_root = home + "/";
  _save_root += ".silver_waffle";
}


SubsystemAssetLoader::~SubsystemAssetLoader()
{
  for(auto json : _jsons)
    delete json.second;

  for(auto objlist : _solar_system_objects)
    {
      for(auto sobj : *objlist.second)
        delete sobj;
      
      delete objlist.second;
    }

  for(auto img : _images)
    delete img.second;

  for(auto m : _meshes)
    delete m.second;

  for(auto f : _fonts)
    delete f.second;

  for(auto p : _shader_programs)
    delete p.second;
}


bool SubsystemAssetLoader::Start()
{
  _asset_paths.clear();
  {
    _asset_paths.push_back(_save_root);

    std::vector<std::string> subdirs
      {
        "",
        "Data",
        "Images"
      };
    for(auto s : subdirs)
      {
        std::string path(_save_root);
        if(!s.empty())
          path += "/" + s;

        std::error_code ec;
        std::filesystem::create_directory(path, ec);
      }
  }
  _asset_paths.push_back(DATADIR);
  
  assert(!AssetLoader);
  AssetLoader = this;

  _texture_quality = Settings->GetInt("texture_quality");
  assert(_texture_quality >= 0 && _texture_quality <= 1);

  _exit_thread = false;
  _thread = new std::thread([this]()
  {
    while(!_exit_thread)
      {
        std::unique_lock<std::mutex> lock(_thread_wakeup_mutex);
        _thread_wakeup.wait(lock);

        LoadNextImage();
        std::this_thread::yield();
      }
  });
  
  return _thread;
}


void SubsystemAssetLoader::StopThreads()
{
  if(_thread)
    {
      _exit_thread = true;
      _thread_wakeup.notify_all();
      _thread->join();
      delete _thread;
    }
}  


void SubsystemAssetLoader::Stop()
{
  if(AssetLoader == this)
    AssetLoader = nullptr;
}


void SubsystemAssetLoader::LoadCache()
{
  {
    auto m = new Mesh(*LoadMesh("BonusCylinder"));
    m->SetShaderProgram(AssetLoader->LoadShaderProgram("SceneObject-Texture"));
    m->SetTexture(0, LoadImage("BonusIcon-2xScore"), true);
    Graphics->QueueUpdateGPU(m);
    _collectibles_meshes[ObjectCollectible::Type::SCORE_MULTIPLIER] = m;
  }
  {
    auto m = new Mesh(*LoadMesh("BonusCylinder"));
    m->SetShaderProgram(AssetLoader->LoadShaderProgram("SceneObject-Texture"));
    m->SetTexture(0, LoadImage("BonusIcon-Shield"), true);
    Graphics->QueueUpdateGPU(m);
    _collectibles_meshes[ObjectCollectible::Type::SHIELD] = m;
  }
}  


const std::string & SubsystemAssetLoader::LoadText(const std::string & filename, bool ignore_not_found_error)
{
  auto it = _text_assets.find(filename);
  if(it != _text_assets.end())
    return (*it).second;

  bool found = false;
  for(auto path : _asset_paths)
    {
      std::ifstream fp(path + "/" + filename);
      if(fp)
        {
          std::string text;
          std::string tmp;
          while(std::getline(fp, tmp))
            text += tmp + '\n';
          
          if(fp.eof())
            {
              std::cout << "Loaded text '" << filename << "'.\n";
              _text_assets[filename] = text;
              return _text_assets[filename];
            }
          else
            std::cerr << "Failed to read '" << filename << "'" << std::endl;

          found = true;
        }
    }

  if(!found)
    if(!ignore_not_found_error)
      std::cout << "Warning, file '" << filename << "' not found error.\n";

  static std::string nothing;
  return nothing;
}


ShaderProgram * SubsystemAssetLoader::LoadShaderProgram(const std::string & name)
{
  auto it = _shader_programs.find(name);
  if(it != _shader_programs.end())
    return (*it).second;

  auto vs = LoadGLSL(name + ".vert", false);
  auto fs = LoadGLSL(name + ".frag", false);
  auto gs = LoadGLSL(name + ".geom", false, true);
  auto sp = new ShaderProgram(vs, fs, gs);
  assert(sp);
#ifndef NDEBUG
  sp->SetName(name);
#endif
  std::cout << "Loaded shader '" << name << "'.\n";

  _shader_programs[name] = sp;

  return sp;
}

json11::Json * SubsystemAssetLoader::LoadJson(const std::string & filename)
{
  auto it = _jsons.find(filename);
  if(it != _jsons.end())
    return (*it).second;

  auto json = new json11::Json();
  std::string json_string = LoadText(filename + ".json", true);
  if(!json_string.empty())
    {
      std::string err;
      *json = json11::Json::parse(json_string, err);
      if(json->is_object())
        std::cout << "Loaded json '" << filename << "'.\n";
      else
        {
          std::cerr << "Error while parsing '" << filename << ".json': " << err << std::endl;
          delete json;
          json = nullptr;
        }
    }
  _jsons[filename] = json;

  return json;
}


Mesh * SubsystemAssetLoader::LoadMesh(const std::string & name, const std::string & shader_prefix)
{
  auto key = name + "-" + shader_prefix;
  
  auto it = _meshes.find(key);
  if(it != _meshes.end())
    return (*it).second;

  auto config = LoadJson("3d-models/" + name);

  unsigned int mesh_options = Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT | Mesh::OPTION_NORMAL;
  if(config && (*config)["blending"].is_bool() && (*config)["blending"].bool_value())
    mesh_options |= Mesh::OPTION_BLEND;
    
  auto mesh = new Mesh(mesh_options);
  assert(mesh);

  bool loaded = false;
  for(auto path : _asset_paths)
    if(mesh->LoadFromFile(path + "/3d-models/" + name + ".dae", shader_prefix))
      {
        loaded = true;
        break;
      }

  assert(loaded);
  if(!loaded)
    return nullptr;

  std::cout << "Loaded mesh '" << name << "'.\n";
  Graphics->QueueUpdateGPU(mesh);
  mesh->CalculateBoundingVolumes();
  _meshes[key] = mesh;

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

      if((*config)["shader"].is_string())
        {
          auto sp = LoadShaderProgram((*config)["shader"].string_value());
          assert(sp);
          mesh->SetShaderProgram(sp, true);
        }
    }
  
  return mesh;
}


Image * SubsystemAssetLoader::LoadImage(const std::string & name)
{
  auto it = _images.find(name);
  if(it != _images.end())
    return (*it).second;

  auto image = new Image(true);
  if(LoadImage(image, name, 0))
    {
      _images[name] = image;
      if(_texture_quality > 0)
        ScheduleLoadImageHighQuality(name);
    }
  else
    {
      if(LoadImage(image, name, 1))
        _images[name] = image;
      else
        {
          delete image;
          image = nullptr;
        }
    }

  assert(image);
  return image;
}


void SubsystemAssetLoader::ScheduleLoadImageHighQuality(const std::string & name)
{
  {
    std::lock_guard lock(_image_requests_mutex);
    _image_requests.emplace_back(name);
  }
  _thread_wakeup.notify_all();
}

SubsystemAssetLoader::Request::Request(const std::string & name)
  : _name(name)
{
}

SubsystemAssetLoader::Request::Request()
{
}



bool SubsystemAssetLoader::LoadNextImage()
{
  Request request;
  {
    std::lock_guard lock(_image_requests_mutex);
    
    if(_image_requests.size() == 0)
      return false;
    
    request = _image_requests.front();
    _image_requests.pop_front();
  }

  assert(_images.find(request._name) != _images.end());
  auto low = _images[request._name];
  assert(low);
  LoadImage(low, request._name, 1);

  return true;
}


bool SubsystemAssetLoader::LoadImage(Image * image, const std::string & name, unsigned int quality)
{
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
  std::string qualityname = "";
  if(quality == 0)
    qualityname = "-low";

  bool rv = false;
  for(auto path : _asset_paths)
    {
      if(image->Load(path + "/Images/" + stripped_name + qualityname + ".png") ||
         image->Load(path + "/Images/" + stripped_name + qualityname + ".jpg")    )
        {
          std::cout << "Loaded image '" << name << "':" << quality << ".\n";
          Graphics->QueueUpdateGPU(image);
          rv = true;
          break;
        }
    }
  return rv;
}


SolarSystemObject * SubsystemAssetLoader::LoadSolarSystemObject(SolarSystemObject::Type type, unsigned int index)
{
  auto it = _solar_system_objects.find(type);
  if(it == _solar_system_objects.end())
    {
      auto config = LoadJson("Data/SolarSystemObjects");
      assert(config);

      std::string arrname = "unknown";
      switch(type)
        {
        case SolarSystemObject::Type::STAR:   arrname = "stars";   break;
        case SolarSystemObject::Type::PLANET: arrname = "planets"; break;
        }
      assert((*config)[arrname].is_array());
      
      auto objs = new std::vector<SolarSystemObject *>();
      for(auto data : (*config)[arrname].array_items())
        {
          auto obj = new SolarSystemObject(type, data);
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



ObjectCollectible * SubsystemAssetLoader::LoadObjectCollectible(int type)
{
  auto typ = static_cast<ObjectCollectible::Type>(type);
  if(typ == ObjectCollectible::Type::NONE)
    return nullptr;
  
  auto collectible = new ObjectCollectible(nullptr);
  switch(typ)
    {
    case ObjectCollectible::Type::NONE:
      assert(false);
      break;
    case ObjectCollectible::Type::SCORE_MULTIPLIER:
    case ObjectCollectible::Type::SHIELD:
      collectible->SetMesh(_collectibles_meshes[typ]);
      break;
    case ObjectCollectible::Type::SCORE_BONUS:
      collectible->SetMesh(LoadMesh("ScoreBonus"));
      break;
    case ObjectCollectible::Type::DAMAGE_MULTIPLIER:
      collectible->SetMesh(LoadMesh("WeaponBoostLiquid"));
      collectible->SetColor(glm::vec3(0.80f, 0.52f, 0.14f));
      break;
    case ObjectCollectible::Type::UPGRADEMATERIAL_ATTACK:
      collectible->SetMesh(LoadMesh("UpgradeMaterial"));
      collectible->SetColor(glm::vec3(1, 0, 0));
      break;
    case ObjectCollectible::Type::UPGRADEMATERIAL_DEFENSE:
      collectible->SetMesh(LoadMesh("UpgradeMaterial"));
      collectible->SetColor(glm::vec3(0, 0, 1));
      break;
    case ObjectCollectible::Type::UPGRADEMATERIAL_PHYSICAL:
      collectible->SetMesh(LoadMesh("UpgradeMaterial"));
      collectible->SetColor(glm::vec3(1, 0.612, 0));
      break;
    case ObjectCollectible::Type::WARP_FUEL:
      collectible->SetMesh(LoadMesh("WarpFuel"));
      break;
    case ObjectCollectible::Type::HUMAN:
      collectible->SetMesh(LoadMesh("Human"));
      break;
    }
  collectible->SetCollisionShape(new CollisionShapeOBB(collectible, collectible->GetMesh()->GetBoundingBoxHalfSize()));

  return collectible;
}


Font * SubsystemAssetLoader::LoadFont(float size)
{
  auto it = _fonts.find(size);
  if(it != _fonts.end())
    return (*it).second;

  std::string name(Settings->GetString("font"));
  auto font = new Font(std::string(DATADIR) + "/" + name, static_cast<unsigned int>(size * 1.15f));
  assert(font);
  
  if(font->Load())
    {
      std::cout << "Loaded font '" << name << "' size " << size << ".\n";
    }
  else
    {
      auto success = font->Generate();
      assert(success);
      std::cout << "Generated font '" << name << "' size " << size << ".\n";
      font->Save();
    }
  
  _fonts[size] = font;
  return font;
}


std::string SubsystemAssetLoader::LoadGLSL(const std::string & filename, bool is_include, bool ignore_not_found_error)
{
  std::string input;

  auto configarr = LoadJson("Data/Shaders");
  assert(configarr);
  assert((*configarr)["shaders"].is_array());

  auto items = (*configarr)["shaders"].array_items();
  bool found = false;
  for(auto i : items)
    if(i["name"].string_value() == filename)
      {
        for(auto f : i["flags"].array_items())
          input += "#define " + f.string_value() + "\n";
        input += LoadGLSL(i["src"].string_value(), true, false);
        found = true;
        break;
      }

  if(!found)
    input = LoadText("Shaders/" + filename, ignore_not_found_error);
  
  if(input.empty())
    return input;

  std::string version;
  if(!is_include)
    version = "#version 330 core\n";
  
  std::string pattern("(^|\n) *#include *\"([-A-Za-z0-9_/.]+)\"");
  std::regex r(pattern, std::regex::extended);
  std::sregex_iterator it(input.cbegin(), input.cend(), r);
  std::sregex_iterator end;

  if(it == end)
    return version + input;
  
  std::string output(version);
  std::string remaining;
  while(it != end)
    {
      output += it->prefix();
      output += "\n";
      output += "/*****************************************************/\n";
      output += "/* BEGIN " + it->str(2) + " */\n";
      output += LoadGLSL(it->str(2), true, false);
      output += "/* END " + it->str(2) + " */\n";
      output += "/*****************************************************/\n";
      remaining = it->suffix();
      it++;
    }
  return output + remaining;
}


const std::string & SubsystemAssetLoader::GetSaveRoot() const
{
  return _save_root;
}
