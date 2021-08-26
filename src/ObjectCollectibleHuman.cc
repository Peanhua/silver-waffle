/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectCollectibleHuman.hh"
#include "CollisionShapeOBB.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


ObjectCollectibleHuman::ObjectCollectibleHuman()
  : ObjectCollectible(nullptr),
    _next_think_time(0),
    _moving_direction(0)
{
  SetBonus(Type::HUMAN, 1);
  SetMesh(AssetLoader->LoadMesh("Human"));
  SetCollisionShape(new CollisionShapeOBB(this, GetMesh()->GetBoundingBoxHalfSize()));
  SetBounciness(1.0);
}


void ObjectCollectibleHuman::Tick(double deltatime)
{
  ObjectCollectible::Tick(deltatime);
  if(!IsAlive())
    return;
  
  _next_think_time -= deltatime;
  if(_next_think_time < 0)
    {
      _next_think_time += 5.0 + 5.0 * static_cast<double>(GetRand());
      _moving_direction = GetRand() < 0.5f ? -1 : 1;
    }  
}


void ObjectCollectibleHuman::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  ObjectCollectible::OnCollision(other, hit_direction);
  if(std::abs(hit_direction.x) < std::abs(hit_direction.z))
    AddImpulse({1.0f * _moving_direction, 0, 0});
}
