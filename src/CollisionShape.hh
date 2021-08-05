#ifndef COLLISION_SHAPE_HH_
#define COLLISION_SHAPE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"

class Mesh;
class Object;


class CollisionShape
{
public:
  enum class Type
    {
      SPHERE,
      OBB
    };

  CollisionShape(Object * owner, Type type);
  virtual ~CollisionShape();

  const Object * GetOwner() const;
  Type GetType() const;
  bool CheckCollision(const CollisionShape & other, glm::vec3 & out_hit_direction);

  const glm::vec3 & GetPosition() const;

  virtual bool CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const = 0;
  virtual bool CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const = 0;

private:
  Object * _owner;
  Type     _type;

#ifdef DEBUG_COLLISION
public:
  Mesh * GetDebugMesh() const;
protected:
  Mesh * _debugmesh;
#endif
};

#endif
