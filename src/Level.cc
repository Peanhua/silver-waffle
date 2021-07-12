#include "Level.hh"
#include "Camera.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ObjectInvader.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>


Level::Level(Scene * scene)
  : _scene(scene),
    _random_generator(0),
    _time(0),
    _boss_buildings_alive(0),
    _destructible_terrain_config(nullptr)
{
}


Level::~Level()
{
}


void Level::Tick(double deltatime)
{
  bool warpspeed = false;
  double dtime = deltatime;
  auto player = _scene->GetPlayer();
  if(player && player->IsAlive())
    {
      auto u = player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
      warpspeed = u->IsActive();
      if(warpspeed)
        dtime *= u->GetValue();
    }
  
  _time += dtime;

  for(unsigned int i = 0; i < _program.size(); i++)
    if(_program[i])
      _program[i] = _program[i]->Tick(_scene, dtime, warpspeed);
}


double Level::GetTime() const
{
  return _time;
}


void Level::Start()
{
  _time = 0;
  _boss_buildings_alive = 0;

  if(_buildings_config.is_array())
    for(auto config : _buildings_config.array_items())
      {
        auto type = static_cast<unsigned int>(config["type"].int_value());
        auto boss = config["boss"].bool_value();
        glm::vec3 offset;
        for(int i = 0; i < 3; i++)
          offset[i] = static_cast<float>(config["offset"][static_cast<unsigned int>(i)].number_value());
        
        {
          glm::vec3 pos = -_scene->GetPlayAreaSize() * 0.5f + _scene->GetPlayAreaSize() * offset;
          auto building = _scene->AddInvader(pos);
          building->SetInvaderType(type);
          building->SetIsSleeping(true);
          building->AddToCollisionChannel(Object::CollisionChannel::TERRAIN);
          building->RemoveFromCollisionChannel(Object::CollisionChannel::ENEMY);
          building->AddCollidesWithChannel(Object::CollisionChannel::ENEMY);
          building->RemoveCollidesWithChannel(Object::CollisionChannel::TERRAIN);
          pos.z = -_scene->GetPlayAreaSize().z * 0.5f + building->GetMesh()->GetBoundingBoxHalfSize().z;
          building->SetPosition(pos);
          
          if(boss)
            {
              _boss_buildings_alive++;
              building->SetOnDestroyed([this](Object * destroyer)
              {
                assert(destroyer == destroyer);
                _boss_buildings_alive--;
              });
            }
        }
      }

  if(_destructible_terrain_config)
    { // For each pixel with alpha > 0, generate a destructible terrain block of relative size.
      auto img = _destructible_terrain_config;
      glm::vec3 blocksize;
      blocksize.x = _scene->GetPlayAreaSize().x / static_cast<float>(img->GetWidth());
      blocksize.y = 4;
      blocksize.z = _scene->GetPlayAreaSize().z / static_cast<float>(img->GetHeight());

      for(unsigned int y = 0; y < img->GetHeight(); y++)
        for(unsigned int x = 0; x < img->GetWidth(); x++)
          {
            auto rgba = img->GetRGBA(x, y);
            if(rgba.a > 0.0f)
              {
                glm::vec3 pos = -_scene->GetPlayAreaSize() * 0.5f;
                pos.x += (static_cast<float>(x) + 0.5f) * blocksize.x;
                pos.y = 0;
                pos.z += (static_cast<float>(y) + 0.5f) * blocksize.z;
                auto building = _scene->AddInvader(pos);
                building->SetInvaderType(3);
                building->SetIsSleeping(true);
                building->AddToCollisionChannel(Object::CollisionChannel::TERRAIN);
                building->RemoveFromCollisionChannel(Object::CollisionChannel::ENEMY);
                building->AddCollidesWithChannel(Object::CollisionChannel::ENEMY);
                building->RemoveCollidesWithChannel(Object::CollisionChannel::TERRAIN);
                building->SetMesh(new Mesh(*building->GetMesh()));
                building->GetMesh()->SetBoundingBoxHalfSize(blocksize * 0.5f);
                building->GetMesh()->SetBoundingSphereRadius(glm::length(blocksize * 0.5f));
                building->GetMesh()->SetAllColor(rgba.rgb());
                building->GetMesh()->UpdateGPU();
                building->GetMesh()->ApplyTransform(glm::scale(blocksize));
                building->SetMaxHealth(building->GetMaxHealth() * static_cast<double>(rgba.a));
                building->SetHealth(building->GetMaxHealth());
                building->CreateCollisionShape(building->GetCollisionShape()->GetType());
              }
          }
    }
}


Level::ProgramEntry::ProgramEntry()
  : _invader_spawn_timer(0),
    _invader_spawn_start_time(0),
    _invader_spawn_stop_time(999999),
    _invader_spawn_interval(-1),
    _boss(false),
    _invader_type(0),
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
  _invader_type = static_cast<unsigned int>(config["invader_type"].int_value());
  if(config["control_program"].is_string())
    SetInvaderControlProgram(config["control_program"].string_value());
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
  if(_boss_buildings_alive > 0)
    return false;

  for(auto p : _program)
    if(p)
      return false;
  
  return true;
}


Level::ProgramEntry * Level::ProgramEntry::Tick(Scene * scene, double deltatime, bool disable_spawning)
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

          if(!disable_spawning)
            {
              auto pos = scene->GetRandomSpawnPosition();
              auto invader = scene->AddInvader(pos);
              assert(invader);
              invader->SetInvaderType(_invader_type);
              if(!_invader_control_program.empty())
                invader->AddNamedControlProgram(_invader_control_program);

              if(_boss)
                {
                  _bosses_alive++;
                  invader->SetOnDestroyed([this](Object * destroyer)
                  {
                    assert(destroyer == destroyer);
                    _bosses_alive--;
                  });
                }
            }
        }
    }
  return this;
}


const std::string & Level::GetName() const
{
  return _name;
}


void Level::LoadConfig(const std::string & filename)
{
  auto levelconfig = AssetLoader->LoadJson(filename);
  assert(levelconfig);
  if(levelconfig)
    {
      auto spawns = (*levelconfig)["spawns"];
      assert(spawns.is_array());
      for(auto spawn : spawns.array_items())
        _program.push_back(new ProgramEntry(spawn));


      _buildings_config = (*levelconfig)["buildings"];

      if((*levelconfig)["terrain"].is_string())
        _destructible_terrain_config = AssetLoader->LoadImage((*levelconfig)["terrain"].string_value());
    }
}
