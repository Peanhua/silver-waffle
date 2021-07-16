#ifndef GAME_STATS_HH_
#define GAME_STATS_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "UpgradeMaterial.hh"
#include <vector>


class GameStats
{
public:
  GameStats();
  
  void         Tick(double deltatime);

  int          GetLives() const;
  void         AdjustLives(int amount);

  double       GetTime() const;
  
  unsigned int GetScore() const;
  void         AddScore(unsigned int amount);
  unsigned int GetScoreMultiplier() const;
  void         SetScoreMultiplier(unsigned int multiplier, double timer);

  UpgradeMaterial * GetUpgradeMaterial(UpgradeMaterial::Type type);
  std::vector<UpgradeMaterial *> & GetUpgradeMaterials();

private:
  int _lives;

  double _time;
  
  unsigned int _score;
  unsigned int _score_multiplier;
  double       _score_multiplier_timer;

  std::vector<UpgradeMaterial *> _upgradematerials;
};

#endif
