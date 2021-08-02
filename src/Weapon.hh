#ifndef WEAPON_HH_
#define WEAPON_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "glm.hh"

class Mesh;
class Object;


class Weapon
{
public:
  Weapon(Object * owner, const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage);

  void   Tick(double deltatime, double coolingmult);
  
  double GetHeat() const;

  bool   IsAutofireOn() const;
  void   SetAutofire(bool on);

  bool   CanFire() const;
  void   Fire();

private:
  Object *  _owner;
  glm::vec3 _location;
  bool      _autofire;
  double    _heat;
  double    _last_fire_timer;
  double    _minimum_firing_interval;
  Mesh *    _projectile;
  glm::vec3 _projectile_direction;
  double    _projectile_initial_velocity;
  double    _projectile_damage;
};

#endif
