/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "CollisionShape.hh"
#include "Object.hh"


CollisionShape::CollisionShape(Object * owner, Type type)
  : _owner(owner),
    _type(type)
#ifdef DEBUG_COLLISION
    , _debugmesh(nullptr)
#endif
{
  assert(_owner);
}


CollisionShape::~CollisionShape()
{
}


const Object * CollisionShape::GetOwner() const
{
  return _owner;
}


CollisionShape::Type CollisionShape::GetType() const
{
  return _type;
}


bool CollisionShape::CheckCollision(const CollisionShape & other, glm::vec3 & out_hit_direction)
{
  switch(other.GetType())
    {
    case Type::SPHERE:
      return CheckCollisionOnSphere(other, out_hit_direction);
    case Type::OBB:
      return CheckCollisionOnOBB(other, out_hit_direction);
    }
  return false;
}


const glm::vec3 & CollisionShape::GetPosition() const
{
  return _owner->GetPosition();
}


#ifdef DEBUG_COLLISION
Mesh * CollisionShape::GetDebugMesh() const
{
  return _debugmesh;
}
#endif
