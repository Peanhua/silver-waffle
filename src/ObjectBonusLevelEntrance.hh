#ifndef OBJECT_BONUS_LEVEL_ENTRANCE_HH_
#define OBJECT_BONUS_LEVEL_ENTRANCE_HH_
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
#include <map>


class ObjectBonusLevelEntrance : public ObjectMovable
{
public:
  ObjectBonusLevelEntrance(Scene * scene, double enemy_difficulty, double warp_fuel_bonus);

  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;
  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
  
private:
  double _enemy_difficulty;
  double _warp_fuel_bonus;
  double _drag_timer;
  bool   _used;
};

#endif
