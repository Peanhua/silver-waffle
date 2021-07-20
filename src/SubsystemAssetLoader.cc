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
#include "SubsystemSettings.hh"
#include <cassert>
#include <iostream>
#include <fstream>


SubsystemAssetLoader * AssetLoader = nullptr;


SubsystemAssetLoader::SubsystemAssetLoader()
  : Subsystem("AssetLoader")
{
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
  assert(!AssetLoader);
  AssetLoader = this;

  return true;
}


void SubsystemAssetLoader::Stop()
{
  if(AssetLoader == this)
    AssetLoader = nullptr;
}


const std::string & SubsystemAssetLoader::LoadText(const std::string & filename, bool ignore_not_found_error)
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
          std::cout << "Loaded text '" << filename << "'.\n";
          _text_assets[filename] = text;
          return _text_assets[filename];
        }
      else
        std::cerr << "Failed to read '" << filename << "'" << std::endl;
    }
  else
    {
      if(!ignore_not_found_error)
        std::cout << "Warning, file '" << filename << "' not found error.\n";
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
  auto gs = LoadText("Shaders/" + name + ".geom", true);
  auto sp = new ShaderProgram(vs, fs, gs);
  assert(sp);
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

  if(mesh->LoadFromFile("3d-models/" + name + ".dae", shader_prefix))
    {
      std::cout << "Loaded mesh '" << name << "'.\n";
      mesh->UpdateGPU();
      mesh->CalculateBoundingVolumes();
      _meshes[key] = mesh;
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
      std::cout << "Loaded image '" << name << "'.\n";
      rv->UpdateGPU(true, true);
      _images[name] = rv;
    }
  else
    {
      std::cout << "Failed to load '" << name << "'.\n";
      delete rv;
      rv = nullptr;
    }
  
  assert(rv);
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
          glm::vec2 ring(0, 0);
          if(data["ring"].is_array())
            {
              auto a = data["ring"].array_items();
              assert(a.size() == 2);
              ring.x = static_cast<float>(a[0].number_value());
              ring.y = static_cast<float>(a[1].number_value());
            }
          
          auto obj = new SolarSystemObject(type,
                                           data["name"].string_value(),
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



ObjectCollectible * SubsystemAssetLoader::LoadObjectCollectible(int type)
{
  if(type == static_cast<int>(ObjectCollectible::Type::NONE))
    return nullptr;
  /*
  auto it = _collectibles.find(type);
  if(it != _collectibles.end())
    return (*it).second;
  */
  auto collectible = new ObjectCollectible(nullptr);
  switch(static_cast<ObjectCollectible::Type>(type))
    {
    case ObjectCollectible::Type::NONE:
      assert(false);
      break;
    case ObjectCollectible::Type::SCORE_BONUS:
      collectible->SetMesh(LoadMesh("ScoreBonus"));
      break;
    case ObjectCollectible::Type::DAMAGE_MULTIPLIER:
      collectible->SetMesh(new Mesh(*LoadMesh("BonusCylinder")));
      collectible->GetMesh()->SetTexture(0, LoadImage("BonusIcon-2xDamage"), true);
      collectible->GetMesh()->UpdateGPU();
      break;
    case ObjectCollectible::Type::SCORE_MULTIPLIER:
      collectible->SetMesh(new Mesh(*LoadMesh("BonusCylinder")));
      collectible->GetMesh()->SetTexture(0, LoadImage("BonusIcon-2xScore"), true);
      collectible->GetMesh()->UpdateGPU();
      break;
    case ObjectCollectible::Type::SHIELD:
      collectible->SetMesh(new Mesh(*LoadMesh("BonusCylinder")));
      collectible->GetMesh()->SetTexture(0, LoadImage("BonusIcon-Shield"), true);
      collectible->GetMesh()->UpdateGPU();
      break;
    case ObjectCollectible::Type::UPGRADEMATERIAL_ATTACK:
      collectible->SetMesh(new Mesh(*LoadMesh("UpgradeMaterial")));
      collectible->GetMesh()->SetAllColor(glm::vec3(1, 0, 0));
      collectible->GetMesh()->UpdateGPU();
      break;
    case ObjectCollectible::Type::UPGRADEMATERIAL_DEFENSE:
      collectible->SetMesh(new Mesh(*LoadMesh("UpgradeMaterial")));
      collectible->GetMesh()->SetAllColor(glm::vec3(0, 0, 1));
      collectible->GetMesh()->UpdateGPU();
      break;
    case ObjectCollectible::Type::UPGRADEMATERIAL_PHYSICAL:
      collectible->SetMesh(new Mesh(*LoadMesh("UpgradeMaterial")));
      collectible->GetMesh()->SetAllColor(glm::vec3(1, 0.612, 0));
      collectible->GetMesh()->UpdateGPU();
      break;
    case ObjectCollectible::Type::WARP_FUEL:
      collectible->SetMesh(LoadMesh("WarpFuel"));
      break;
    }
  collectible->SetCollisionShape(new CollisionShapeOBB(collectible, collectible->GetMesh()->GetBoundingBoxHalfSize()));

  //_collectibles[type] = collectible;
  return collectible;
}


Font * SubsystemAssetLoader::LoadFont(float size)
{
  auto it = _fonts.find(size);
  if(it != _fonts.end())
    return (*it).second;

  std::string name(Settings->GetString("font"));
  auto font = new Font(name, static_cast<unsigned int>(size * 1.15f));
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

