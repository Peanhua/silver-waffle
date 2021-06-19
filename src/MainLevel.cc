#include "MainLevel.hh"
#include "ObjectBonusLevelEntrance.hh"
#include "ObjectCollectible.hh"
#include "ObjectPlanet.hh"
#include "Scene.hh"
#include "SolarSystemObject.hh"
#include "SubsystemAssetLoader.hh"


#define SCALE 100.0


MainLevel::MainLevel(Scene * scene, const SolarSystemObject * planet)
  : Level(scene),
    _solar_system_object(planet)
{
  _planet_size = planet->GetRelativeSize() * SCALE;
  _name = planet->GetName();

  auto levelconfig = AssetLoader->LoadJson("Data/Level-" + _name);
  assert(levelconfig);
  if(levelconfig)
    {
      auto spawns = (*levelconfig)["spawns"];
      assert(spawns.is_array());
      for(auto spawn : spawns.array_items())
        _program.push_back(new ProgramEntry(spawn));
    }

  double len = 0.0;
  for(auto p : _program)
    if(p)
      len = std::max(len, p->GetRemainingTime());
  _planet_position_start = len;
}


bool MainLevel::IsFinished() const
{
  for(auto p : _program)
    if(p)
      return false;
  return true;
}



void MainLevel::Start()
{
  Level::Start();

  auto planet = dynamic_cast<ObjectPlanet *>(_solar_system_object->CreateSceneObject(_scene, SCALE));
  assert(planet);
 
  planet->SetPosition(glm::vec3(0, _planet_position_start, -_planet_size * 0.5));
  planet->SetVelocity(glm::vec3(0, -1, 0));
  planet->SetAngularVelocity(glm::angleAxis(glm::radians(1.5f), glm::vec3(0, 0, 1)), 1.0);
  
  _scene->AddPlanet(planet);

  {
    auto rand = [this]()
    {
      return (static_cast<float>(_random_generator()) - static_cast<float>(_random_generator.min())) / static_cast<float>(_random_generator.max());
    };
    
    int blecount = 3;
    for(int i = 0; i < blecount; i++)
      {
        auto ble = new ObjectBonusLevelEntrance(_scene, rand(), rand());

        float min_distance_from_walls = 1.0f;
        auto max_x = _scene->GetPlayAreaSize().x * 0.5f - min_distance_from_walls;
        float speed = 5.0;

        ble->SetVelocity(glm::vec3(0, -speed, 0));

        float y =
          speed
          * static_cast<float>(_planet_position_start * 0.95)
          * static_cast<float>(i + 1) / static_cast<float>(blecount + 1);

        glm::vec3 pos(-max_x + rand() * max_x * 2.0f, y, 0.0f);
        
        _scene->AddObject(ble, pos);
      }
  }
}
