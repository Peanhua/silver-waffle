#include "ScreenPlanetLevel.hh"
#include "Camera.hh"
#include "PlanetLevel.hh"
#include "ScenePlanet.hh"


ScreenPlanetLevel::ScreenPlanetLevel(ScreenLevel * parent)
  : ScreenLevel(parent)
{
  _camera = new Camera();
  _camera->SetFOV(60);
  _camera->SetClippingPlanes(0.01, 10000.0);

  _scene = new ScenePlanet();

  Initialize();
}


void ScreenPlanetLevel::SetupLevels()
{
  _levels.clear();

  auto level = new PlanetLevel(_scene);
  _levels.push_back(level);

  ScreenLevel::SetupLevels();
}
