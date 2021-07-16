#ifndef COLLISION_SHAPE_SPHERE_HH_
#define COLLISION_SHAPE_SPHERE_HH_
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


class CollisionShapeSphere : public CollisionShape
{
public:
  CollisionShapeSphere(Object * owner, double radius);
  
  bool CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const override;
  bool CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const override;

  float GetRadius() const;
  
private:
  float _radius;
};

#endif
