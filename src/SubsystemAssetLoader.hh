#ifndef SUBSYSTEM_ASSET_LOADER_HH_
#define SUBSYSTEM_ASSET_LOADER_HH_

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
  
  bool Start() override;
  void Stop() override;

  Font *              LoadFont(float size);
  const std::string & LoadText(const std::string & filename);
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
