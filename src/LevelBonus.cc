/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "LevelBonus.hh"

LevelBonus::LevelBonus(Scene * scene, double enemy_difficulty, double warp_fuel_bonus)
  : Level(scene)
{
  _name = "Bonus Level " + std::to_string(enemy_difficulty) + "/" + std::to_string(warp_fuel_bonus);
  _halt_without_program = true;

  LoadConfig("Data/Level-Bonus");
}
