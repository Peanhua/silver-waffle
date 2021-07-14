#include "ScreenLevelPlanet.hh"
#include "Camera.hh"
#include "ObjectSpaceship.hh"
#include "LevelPlanet.hh"
#include "ScenePlanet.hh"


ScreenLevelPlanet::ScreenLevelPlanet(ScreenLevel * parent, SolarSystemObject * planet)
  : ScreenLevel(parent),
    _planet(planet)
{
  _camera = new Camera();
  _camera->SetFOV(60);
  _camera->SetClippingPlanes(0.01, 10000.0);
  _camera->SetPosition({0, -20, 0});

  _scene = new ScenePlanet();

  Initialize();
}


void ScreenLevelPlanet::SetupLevels()
{
  _levels.clear();

  auto level = new LevelPlanet(_scene, _planet);
  _levels.push_back(level);

  ScreenLevel::SetupLevels();
}


void ScreenLevelPlanet::Tick(double deltatime)
{
  auto pos = _scene->GetPlayer()->GetPosition();
  _camera->SetPosition({pos.x, -20, pos.z});
  _camera->SetTargetPosition({pos.x, 0, pos.z});

  ScreenLevel::Tick(deltatime);
}
