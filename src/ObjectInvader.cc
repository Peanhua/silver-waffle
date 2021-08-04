/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectInvader.hh"
#include "Loot.hh"
#include "SubsystemAssetLoader.hh"


ObjectInvader::ObjectInvader(Scene * scene, unsigned int random_seed, unsigned int type)
  : ObjectSpaceship(scene, random_seed)
{
  AddToCollisionChannel(CollisionChannel::ENEMY);
  AddCollidesWithChannel(CollisionChannel::PLAYER);
  RotateYaw(180.0);

  auto config = AssetLoader->LoadJson("Data/Invaders");
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
  
  RemoveWeapons();
  assert(d["weapons"].is_number());
  for(int i = 0; i < d["weapons"].number_value(); i++)
    AddWeapon();

  assert(d["health"].is_number());
  SetMaxHealth(d["health"].number_value());

  if(d["hit_impulse"].is_bool())
    EnableHitImpulse(d["hit_impulse"].bool_value());

  ClearLoot();
  assert(d["loot"].is_array());
  auto loots = d["loot"].array_items();
  for(auto loot : loots)
    {
      assert(loot.is_string());
      auto lootjson = AssetLoader->LoadJson("Data/Loot-" + loot.string_value());
      AddLoot(new Loot(lootjson));
    }
  assert(d["shield_chance"].is_number());
  if(static_cast<double>(GetRand()) < d["shield_chance"].number_value())
    {
      auto u = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
      u->Activate(50.0 + GetMaxHealth() * static_cast<double>(GetRand()), 9999);
    }
  
  SetHealth(GetMaxHealth());

  if(d["random_rotation"].is_bool() && d["random_rotation"].bool_value())
    {
      auto rotangle = glm::normalize(glm::vec3(GetRand() * 2.0f - 1.0f,
                                               GetRand() * 2.0f - 1.0f,
                                               GetRand() * 2.0f - 1.0f));
      SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1f + GetRand() * 10.0f);
    }
}
