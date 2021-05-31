#ifndef SUBSYSTEM_ASSET_LOADER_HH_
#define SUBSYSTEM_ASSET_LOADER_HH_

#include "Subsystem.hh"
#include <json11.hpp>
#include <map>
#include <vector>

class Image;
class Mesh;
class ShaderProgram;
class SolarSystemObject;


class SubsystemAssetLoader : public Subsystem
{
public:
  SubsystemAssetLoader();
  
  bool Start() override;
  void Stop() override;

  const std::string & LoadText(const std::string & filename);
  json11::Json *      LoadJson(const std::string & filename);
  ShaderProgram *     LoadShaderProgram(const std::string & name);
  Mesh *              LoadMesh(const std::string & name);
  Image *             LoadImage(const std::string & name);
  SolarSystemObject * LoadSolarSystemObject(int type, unsigned int index);

private:
  std::map<std::string, std::string>     _text_assets;
  std::map<std::string, json11::Json *>  _jsons;
  std::map<std::string, ShaderProgram *> _shader_programs;
  std::map<std::string, Mesh *>          _meshes;
  std::map<std::string, Image *>         _images;
  std::map<int, std::vector<SolarSystemObject *> *> _solar_system_objects;
};

extern SubsystemAssetLoader * AssetLoader;

#endif
