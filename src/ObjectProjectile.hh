#ifndef OBJECT_PROJECTILE_HH_
#define OBJECT_PROJECTILE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "ObjectMovable.hh"

class ObjectProjectile : public ObjectMovable
{
public:
  ObjectProjectile(Scene * scene);

  void Tick(double deltatime) override;
  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;

  void Activate(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);

  Object * GetOwner() const;
  void     SetOwner(Object * owner);
  double   GetDamage() const;
  
private:
  Object * _owner;
  double   _damage;
  double   _lifetime_left;
};

#endif
