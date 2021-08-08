/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "CollisionShapeSphere.hh"
#include "Mesh.hh"
#include "Object.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"


CollisionShapeSphere::CollisionShapeSphere(Object * owner, double radius)
  : CollisionShape(owner, Type::SPHERE),
    _radius(static_cast<float>(radius))
{
#ifdef DEBUG_COLLISION
  _debugmesh = new Mesh(*AssetLoader->LoadMesh("Sphere", "Generic"));
  _debugmesh->ApplyTransform(glm::scale(glm::vec3(radius, radius, radius) * 2.0f));
  _debugmesh->SetAllColor({1, 0.5, 0.5}, true);
  Graphics->QueueUpdateGPU(_debugmesh);
#endif
}


bool CollisionShapeSphere::CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const
{
  auto o = static_cast<const CollisionShapeSphere &>(other);
  
  auto distance = glm::distance(GetPosition(), o.GetPosition());
  if(distance > _radius + o._radius)
    return false;

  out_hit_direction = glm::normalize(o.GetPosition() - GetPosition());
  return true;
}


bool CollisionShapeSphere::CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const
{
  auto rv = other.CheckCollisionOnSphere(*this, out_hit_direction);
  out_hit_direction *= -1;
  return rv;
}


float CollisionShapeSphere::GetRadius() const
{
  return _radius;
}
