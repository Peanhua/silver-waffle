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

#include "SpaceshipUpgrade.hh"
#include "UpgradeMaterial.hh"
#include <map>
#include <utility>
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

  void         OnHumanCollected();
  void         OnHumansSaved(unsigned int count);
  unsigned int GetHumansCollected() const;
  unsigned int GetHumansSaved() const;

  std::pair<bool, SpaceshipUpgrade::Type> GetRandomUnlockableSpaceshipUpgrade(float random);
  void         UnlockSpaceshipUpgrade(SpaceshipUpgrade::Type type);
  bool         IsSpaceshipUpgradeAvailable(SpaceshipUpgrade::Type type);
  
private:
  int _lives;

  double _time;
  
  unsigned int _score;
  unsigned int _score_multiplier;
  double       _score_multiplier_timer;

  unsigned int _total_humans_collected;
  unsigned int _total_humans_saved;

  std::vector<UpgradeMaterial *> _upgradematerials;
  std::map<SpaceshipUpgrade::Type, bool> _spaceship_upgrade_blueprints;
};

#endif
