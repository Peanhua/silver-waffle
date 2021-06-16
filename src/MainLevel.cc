#include "MainLevel.hh"
#include "ObjectBonusLevelEntrance.hh"
#include "ObjectCollectible.hh"
#include "ObjectPlanet.hh"
#include "Scene.hh"
#include "SolarSystemObject.hh"


#define SCALE = 100.0;


MainLevel::MainLevel(Scene * scene, const SolarSystemObject * planet)
  : Level(scene),
    _solar_system_object(planet)
{
  _planet_size = planet->GetRelativeSize() * SCALE;
  _planet_position_start = 200.0 + _planet_size * 9.0;


  auto end_of_time = _planet_position_start - 2.0 * (10 + 53.0); // 2.0 is the speed of an invader, 53 is distance from invader spawn to player, 10 extra
  
  auto e = new ProgramEntry();
  e->SetStartTime(0.0);
  e->SetStopTime(30.0);
  e->SetSpawnInterval(0.5);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(60.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(2.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(70.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(3.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(90.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(4.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(120.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(2.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(end_of_time - 50.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(0.05);
  _program.push_back(e);
}


bool MainLevel::IsFinished() const
{
  return GetTime() > GetTotalTime();
}


double MainLevel::GetTotalTime() const
{
  return 20.0 + _planet_position_start + _planet_size * 0.35;
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
    
    int blecount = 10 + std::clamp(static_cast<int>(rand() * 10.0f), 0, 20);
    for(int i = 0; i < blecount; i++)
      {
        auto ble = new ObjectBonusLevelEntrance(_scene, rand(), rand());

        float min_distance_from_walls = 2.0f;
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
