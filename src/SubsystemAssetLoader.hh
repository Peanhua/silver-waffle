#ifndef SUBSYSTEM_ASSET_LOADER_HH_
#define SUBSYSTEM_ASSET_LOADER_HH_

#include "Subsystem.hh"
#include <map>

class ShaderProgram;
class Mesh;
class Image;

class SubsystemAssetLoader : public Subsystem
{
public:
  SubsystemAssetLoader();
  
  bool Start() override;
  void Stop() override;

  const std::string & LoadText(const std::string & filename);
  ShaderProgram *     LoadShaderProgram(const std::string & name);
  Mesh *              LoadMesh(const std::string & name);
  Image *             LoadImage(const std::string & name);

private:
  std::map<std::string, std::string>     _text_assets;
  std::map<std::string, ShaderProgram *> _shader_programs;
  std::map<std::string, Mesh *>          _meshes;
  std::map<std::string, Image *>         _images;
};

extern SubsystemAssetLoader * AssetLoader;

#endif
