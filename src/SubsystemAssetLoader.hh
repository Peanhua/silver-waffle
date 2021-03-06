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
#include "ObjectCollectible.hh"
#include "SolarSystemObject.hh"
#include <json11.hpp>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <map>
#include <mutex>
#include <thread>
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
  
  bool Start()       override;
  void StopThreads() override;
  void Stop()        override;

  void LoadCache();

  Font *              LoadFont(float size);
  const std::string & LoadText(const std::string & filename, bool ignore_not_found_error = false);
  json11::Json *      LoadJson(const std::string & filename);
  ShaderProgram *     LoadShaderProgram(const std::string & name);
  std::string         LoadGLSL(const std::string & filename, bool is_include, bool ignore_not_found_error = false);
  Mesh *              LoadMesh(const std::string & name, const std::string & shader_prefix = "SceneObject");
  Image *             LoadImage(const std::string & name);
  SolarSystemObject * LoadSolarSystemObject(SolarSystemObject::Type type, unsigned int index);
  ObjectCollectible * LoadObjectCollectible(int type);

  const std::string & GetSaveRoot() const;
  
private:
  int               _texture_quality;
  std::thread *     _thread;
  std::atomic<bool> _exit_thread;
  std::mutex        _thread_wakeup_mutex;
  std::condition_variable _thread_wakeup;
  std::vector<std::string> _asset_paths;
  std::string       _save_root;
  
  std::map<std::string, std::string>     _text_assets;
  std::map<std::string, json11::Json *>  _jsons;
  std::map<std::string, ShaderProgram *> _shader_programs;
  std::map<std::string, Mesh *>          _meshes;
  std::map<std::string, Image *>         _images;
  std::map<SolarSystemObject::Type, std::vector<SolarSystemObject *> *> _solar_system_objects;
  std::map<ObjectCollectible::Type, Mesh *> _collectibles_meshes;
  std::map<float, Font *>                _fonts;

  bool LoadImage(Image * image, const std::string & name, unsigned int quality);
  void ScheduleLoadImageHighQuality(const std::string & name);
  bool LoadNextImage();


  class Request
  {
  public:
    Request();
    Request(const std::string & name);
    std::string _name;
  };
  std::deque<Request> _image_requests;
  std::mutex          _image_requests_mutex;
};

extern SubsystemAssetLoader * AssetLoader;

#endif
