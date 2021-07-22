/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Level.hh"
#include "Camera.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ObjectBuilding.hh"
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

  if(_destructible_terrain_config)
    { // For each pixel with alpha > 0, generate a destructible terrain block of relative size.
      auto img = _destructible_terrain_config;
      glm::vec3 blocksize;
      blocksize.x = _scene->GetPlayAreaSize().x / static_cast<float>(img->GetWidth());
      blocksize.y = 4;
      blocksize.z = _scene->GetPlayAreaSize().z / static_cast<float>(img->GetHeight());

      Mesh * mesh = nullptr;
      for(unsigned int y = 0; y < img->GetHeight(); y++)
        for(unsigned int x = 0; x < img->GetWidth(); x++)
          {
            auto rgba = img->GetRGBA(x, y);
            if(rgba.a > 0.0f)
              {
                glm::vec3 pos = -_scene->GetPlayAreaSize() * 0.5f + glm::vec3(blocksize.x, 0, blocksize.z) * 0.5f;
                pos.x += static_cast<float>(x) * blocksize.x;
                pos.y = 0;
                pos.z += static_cast<float>(y) * blocksize.z;
                auto block = new ObjectBuilding(_scene, static_cast<unsigned int>(_random_generator()), 1);
                _scene->AddObject(block, pos);
                block->SetCollidesWithChannels(0);
                if(!mesh)
                  {
                    mesh = new Mesh(*block->GetMesh());
                    mesh->UpdateGPU();
                    mesh->SetBoundingBoxHalfSize(blocksize * 0.5f);
                    mesh->SetBoundingSphereRadius(glm::length(blocksize * 0.5f));
                    mesh->ApplyTransform(glm::scale(blocksize));
                  }
                block->SetMesh(mesh);
                block->SetColor(rgba.rgb());
                block->SetMaxHealth(block->GetMaxHealth() * static_cast<double>(rgba.a));
                block->SetHealth(block->GetMaxHealth());
                block->SetUseHealth(rgba.a < 1.0f);
                block->CreateCollisionShape(block->GetCollisionShape()->GetType());
              }
          }
    }
  
  if(_buildings_config.is_array())
    for(auto config : _buildings_config.array_items())
      {
        auto type = static_cast<unsigned int>(config["type"].int_value());
        auto boss = config["boss"].bool_value();
        glm::vec3 offset;
        for(int i = 0; i < 3; i++)
          offset[i] = static_cast<float>(config["offset"][static_cast<unsigned int>(i)].number_value());
        
        {
          auto building = new ObjectBuilding(_scene, static_cast<unsigned int>(_random_generator()), type);
          _scene->AddObject(building);

          glm::vec3 pos = -_scene->GetPlayAreaSize() * 0.5f + _scene->GetPlayAreaSize() * offset;
          pos = _scene->GetClosestGroundSurface(pos);
          pos.z += building->GetMesh()->GetBoundingBoxHalfSize().z;
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
}


Level::ProgramEntry::ProgramEntry()
  : _invader_spawn_timer(0),
    _invader_spawn_start_time(0),
    _invader_spawn_stop_time(999999),
    _invader_spawn_interval(-1),
    _boss(false),
    _invader_type(0),
    _bosses_alive(0),
    _spawn_count(0),
    _max_spawn_count(0),
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
  if(config["max"].is_number())
     _max_spawn_count = static_cast<unsigned int>(config["max"].number_value());
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
            if(_max_spawn_count == 0 || _spawn_count < _max_spawn_count)
              {
                auto pos = scene->GetRandomSpawnPosition();
                auto invader = scene->AddInvader(_invader_type, pos);
                assert(invader);
                _spawn_count++;

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
      if(spawns.is_array())
        for(auto spawn : spawns.array_items())
          _program.push_back(new ProgramEntry(spawn));


      _buildings_config = (*levelconfig)["buildings"];

      if((*levelconfig)["terrain"].is_string())
        _destructible_terrain_config = AssetLoader->LoadImage((*levelconfig)["terrain"].string_value());
    }
}
