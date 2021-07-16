#ifndef SUBSYSTEM_ASSET_LOADER_HH_
#define SUBSYSTEM_ASSET_LOADER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Subsystem.hh"
#include "SolarSystemObject.hh"
#include <json11.hpp>
#include <map>
#include <vector>

class Font;
class Image;
class Mesh;
class ObjectCollectible;
class ShaderProgram;


class SubsystemAssetLoader : public Subsystem
{
public:
  SubsystemAssetLoader();
  ~SubsystemAssetLoader();
  
  bool Start() override;
  void Stop() override;

  Font *              LoadFont(float size);
  const std::string & LoadText(const std::string & filename, bool ignore_not_found_error = false);
  json11::Json *      LoadJson(const std::string & filename);
  ShaderProgram *     LoadShaderProgram(const std::string & name);
  Mesh *              LoadMesh(const std::string & name, const std::string & shader_prefix = "SceneObject");
  Image *             LoadImage(const std::string & name);
  SolarSystemObject * LoadSolarSystemObject(SolarSystemObject::Type type, unsigned int index);
  ObjectCollectible * LoadObjectCollectible(int type);

private:
  std::map<std::string, std::string>     _text_assets;
  std::map<std::string, json11::Json *>  _jsons;
  std::map<std::string, ShaderProgram *> _shader_programs;
  std::map<std::string, Mesh *>          _meshes;
  std::map<std::string, Image *>         _images;
  std::map<SolarSystemObject::Type, std::vector<SolarSystemObject *> *> _solar_system_objects;
  std::map<int, ObjectCollectible *>     _collectibles;
  std::map<float, Font *>                _fonts;
};

extern SubsystemAssetLoader * AssetLoader;

#endif
