/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectBuilding.hh"
#include "Loot.hh"
#include "Mesh.hh"
#include "NavigationMap.hh"
#include "ObjectInvader.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSfx.hh"


ObjectBuilding::ObjectBuilding(Scene * scene, unsigned int random_seed, unsigned int type)
  : Object(scene, random_seed),
    _is_spaceport(false)
{
  SetIsSleeping(true);
  SetTickingRequiresPlayerAlive(true);
  SetTickingRequiresPlayerVisibility(true);
  
  SetCollisionChannels(0);
  AddToCollisionChannel(Object::CollisionChannel::TERRAIN);
  SetCollidesWithChannels(0);
  auto config = AssetLoader->LoadJson("Data/Buildings");
  assert(config->is_object());
  assert((*config)["definitions"].is_array());
  auto defs = (*config)["definitions"].array_items();
  assert(type < defs.size());

  auto d = defs[type];
  
  assert(d["mesh"].is_string());
  auto mesh = AssetLoader->LoadMesh(d["mesh"].string_value());
  assert(mesh);
  SetMesh(mesh);

  CollisionShape::Type ct = CollisionShape::Type::OBB;
  if(d["collision_shape"].is_string())
    {
      if(d["collision_shape"] == "obb")
        ct = CollisionShape::Type::OBB;
      else if(d["collision_shape"] == "sphere")
        ct = CollisionShape::Type::SPHERE;
      else
        assert(false); // unknown collision shape
    }

  CreateCollisionShape(ct);

  assert(d["health"].is_number());
  SetMaxHealth(d["health"].number_value());
  SetHealth(GetMaxHealth());

  if(d["spawn"].is_object())
    {
      assert(d["spawn"]["type"].is_number());
      assert(d["spawn"]["interval"].is_number());
      assert(d["spawn"]["max"].is_number());
      assert(d["spawn"]["control_program"].is_string());
      AddSpawn(static_cast<unsigned int>(d["spawn"]["type"].int_value()),
               d["spawn"]["interval"].number_value(),
               static_cast<unsigned int>(d["spawn"]["max"].int_value()),
               d["spawn"]["control_program"].string_value());
      SetIsSleeping(false);
    }

  if(d["spaceport"].is_bool() && d["spaceport"].bool_value())
    _is_spaceport = true;

  if(d["loot"].is_array())
    {
      auto loots = d["loot"].array_items();
      for(auto loot : loots)
        {
          assert(loot.is_string());
          auto lootjson = AssetLoader->LoadJson("Data/Loot-" + loot.string_value());
          AddLoot(new Loot(lootjson));
        }
    }
}


void ObjectBuilding::Tick(double deltatime)
{
  Object::Tick(deltatime);
  
  auto scene = GetScene();
  if(!scene)
    return;
  
  for(auto & spawn : _spawns)
    if(spawn.count < spawn.max_count)
      {
        spawn.next_spawn_time -= deltatime;
        if(spawn.next_spawn_time < 0.0)
          {
            spawn.next_spawn_time += spawn.interval;
            spawn.count++;
            
            auto invader = scene->AddInvader(spawn.type, {0, 0, 0});
            
            auto pos = GetPosition();
            pos.z += static_cast<float>(GetMesh()->GetBoundingSphereRadius()); // todo: Query the correct (sphere or box) bounding z.
            pos.z += invader->GetMesh()->GetBoundingBoxHalfSize().z;
            pos.z += 0.01f;
            invader->SetPosition(pos);
            
            invader->AddImpulse({(GetRand() - 0.5f) * 3, 0, 3});
            invader->AddEngine({0, -1, 0}, 1.0 + 2.0 * static_cast<double>(GetRand()));
            invader->SetEngineThrottle(0, 1.0);
            invader->AddNamedControlProgram(spawn.control_program);
            invader->SetDrag(0.5);
          }
      }
}


void ObjectBuilding::AddSpawn(unsigned int spawn_type, double spawn_interval, unsigned int max_count, const std::string & control_program)
{
  _spawns.emplace_back(spawn_type, spawn_interval, spawn_interval, max_count, control_program);
}


bool ObjectBuilding::GetIsSpaceport() const
{
  return _is_spaceport;
}


void ObjectBuilding::OnDestroyed(Object * destroyer)
{
  Sounds->PlaySoundEffect("explosion.big", GetPosition());

  auto scene = GetScene();
  if(scene)
    {
      auto navmap = scene->GetNavigationMap();
      if(navmap)
        {
          auto pos = navmap->WorldToNavigation(GetPosition());
          navmap->Set(pos, '.');
        }
    }
  
  Object::OnDestroyed(destroyer);
}

