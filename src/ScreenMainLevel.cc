#include "ScreenMainLevel.hh"
#include "Camera.hh"
#include "MainLevel.hh"
#include "SceneSpace.hh"
#include "SubsystemAssetLoader.hh"


ScreenMainLevel::ScreenMainLevel(ScreenLevel * parent)
  : ScreenLevel(parent)
{
  _camera = new Camera();
  _camera->SetFOV(60);
  _camera->SetClippingPlanes(0.01, 10000.0);
  
  _scene = new SceneSpace();

  Initialize();
}


void ScreenMainLevel::SetupLevels()
{
  bool done = false;
  for(unsigned int i = 0; !done; i++)
    {
      auto sobj = AssetLoader->LoadSolarSystemObject(SolarSystemObject::Type::PLANET, i);
      if(sobj)
        _levels.push_back(new MainLevel(_scene, sobj));
      else
        done = true;
    }

  ScreenLevel::SetupLevels();
}
