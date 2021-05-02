#ifndef SUBSYSTEM_ASSET_LOADER_HH_
#define SUBSYSTEM_ASSET_LOADER_HH_

#include "Subsystem.hh"
#include <string>
#include <map>

class SubsystemAssetLoader : public Subsystem
{
public:
  SubsystemAssetLoader();
  
  bool Start() override;
  void Stop() override;

  const std::string & LoadText(const std::string & filename);

private:
  std::map<std::string, std::string> _text_assets;
};

extern SubsystemAssetLoader * AssetLoader;

#endif
