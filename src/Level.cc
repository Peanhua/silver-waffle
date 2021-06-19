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

  for(unsigned int i = 0; i < _program.size(); i++)
    if(_program[i])
      _program[i] = _program[i]->Tick(_scene, _random_generator, deltatime);
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
    _invader_spawn_interval(1.0),
    _boss(false),
    _bosses_alive(0),
    _next(nullptr)
{
}

Level::ProgramEntry::ProgramEntry(const json11::Json & config)
  : ProgramEntry()
{
  if(config["start"].is_number())
    SetStartTime(config["start"].number_value());
  if(config["stop"].is_number())
    SetStopTime(config["stop"].number_value());
  if(config["interval"].is_number())
    SetSpawnInterval(config["interval"].number_value());
  if(config["control_program"].is_string())
    SetInvaderControlProgram(config["control_program"].string_value());
  if(config["boss"].is_bool())
    _boss = config["boss"].bool_value();
  if(config["next"].is_object())
    SetNext(new ProgramEntry(config["next"]));
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


void Level::ProgramEntry::SetInvaderControlProgram(const std::string & program_name)
{
  _invader_control_program = program_name;
}


void Level::ProgramEntry::SetNext(ProgramEntry * next)
{
  _next = next;
}


double Level::ProgramEntry::GetRemainingTime() const
{
  auto remaining = _invader_spawn_stop_time;
  if(_next)
    remaining += _next->GetRemainingTime();
  return remaining;
}


bool Level::IsFinished() const
{
  for(auto p : _program)
    if(p)
      return false;
  return true;
}



Level::ProgramEntry * Level::ProgramEntry::Tick(Scene * scene, std::mt19937_64 & random_generator, double deltatime)
{
  if(_invader_spawn_stop_time < 0.0)
    {
      if(_bosses_alive > 0)
        return this;
      else
        return _next;
    }
  _invader_spawn_stop_time -= deltatime;
  
  if(_invader_spawn_start_time > 0.0)
    {
      _invader_spawn_start_time -= deltatime;
      return this;
    }

  if(_invader_spawn_interval > 0.0)
    {
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
          assert(invader);
          if(rand() < 0.2f)
            {
              auto u = invader->GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
              u->Activate(50.0f + 150.0f * rand(), 9999);
            }
          if(!_invader_control_program.empty())
            invader->AddNamedControlProgram(_invader_control_program);
          if(_boss)
            {
              _bosses_alive++;
              invader->SetOnDestroyed([this](Object * destroyer)
              {
                _bosses_alive--;
              });
            }
        }
    }
  return this;
}


const std::string & Level::GetName() const
{
  return _name;
}

