/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ScreenLevelBonus.hh"
#include "LevelBonus.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


ScreenLevelBonus::ScreenLevelBonus(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus)
  : ScreenLevelSpace(parent),
    _enemy_difficulty(enemy_difficulty),
    _warp_fuel_bonus(warp_fuel_bonus)
{
}


void ScreenLevelBonus::SetupLevels()
{
  _levels.clear();
  auto level = new LevelBonus(_scene, _enemy_difficulty, _warp_fuel_bonus);
  _levels.push_back(level);

  ScreenLevel::SetupLevels();
}
