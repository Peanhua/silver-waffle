#ifndef SCREEN_LEVEL_BONUS_HH_
#define SCREEN_LEVEL_BONUS_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "ScreenLevelSpace.hh"


class ScreenLevelBonus : public ScreenLevelSpace
{
public:
  ScreenLevelBonus(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus);
  
  void SetupLevels() override;

private:
  double _enemy_difficulty;
  double _warp_fuel_bonus;
};

#endif
