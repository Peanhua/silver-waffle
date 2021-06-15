#include "Level.hh"
#include "Camera.hh"
#include "ObjectInvader.hh"
#include "Scene.hh"
#include <cassert>



Level::Level(Scene * scene)
  : _scene(scene),
    _random_generator(0),
    _time(0)
{
}


Level::~Level()
{
}


void Level::Tick(double deltatime)
{
  _time += deltatime;

  for(auto e : _program)
    e->Tick(_scene, _random_generator, deltatime);
}


double Level::GetTime() const
{
  return _time;
}


void Level::Start()
{
  _time = 0;
}


Level::ProgramEntry::ProgramEntry()
  : _invader_spawn_timer(0),
    _invader_spawn_start_time(0),
    _invader_spawn_stop_time(999999),
    _invader_spawn_interval(1.0)
{
}


void Level::ProgramEntry::SetStartTime(double time)
{
  _invader_spawn_start_time = time;
}


void Level::ProgramEntry::SetStopTime(double time)
{
  _invader_spawn_stop_time = time;
}


void Level::ProgramEntry::SetSpawnInterval(double interval)
{
  _invader_spawn_interval = interval;
}


void Level::ProgramEntry::Tick(Scene * scene, std::mt19937_64 & random_generator, double deltatime)
{
  if(_invader_spawn_stop_time < 0.0)
      return;
  _invader_spawn_stop_time -= deltatime;
  
  if(_invader_spawn_start_time > 0.0)
    {
      _invader_spawn_start_time -= deltatime;
      return;
    }

  _invader_spawn_timer += deltatime;
  if(_invader_spawn_timer > _invader_spawn_interval)
    {
      _invader_spawn_timer -= _invader_spawn_interval;
      
      auto rand = [&random_generator]()
      {
        return (static_cast<float>(random_generator()) - static_cast<float>(random_generator.min())) / static_cast<float>(random_generator.max());
      };
      
      const auto max_x = scene->GetPlayAreaSize().x * 0.5f;
      auto invader = scene->AddInvader(glm::vec3(-max_x + rand() * max_x * 2.0f, 40, 0));
      if(invader && rand() < 0.2f)
        {
          auto u = invader->GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
          u->Activate(50.0f + 150.0f * rand(), 9999);
        }
    }
}
