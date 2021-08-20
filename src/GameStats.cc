/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "GameStats.hh"
#include "SubsystemSettings.hh"
#include <cassert>


GameStats::GameStats()
  : _lives(3),
    _blueprint_points(0),
    _time(0),
    _score(0),
    _score_multiplier(1),
    _score_multiplier_timer(0),
    _total_humans_collected(0),
    _total_humans_saved(0)
{
  _upgradematerials.push_back(new UpgradeMaterial(UpgradeMaterial::Type::ATTACK,   "Material A"));
  _upgradematerials.push_back(new UpgradeMaterial(UpgradeMaterial::Type::DEFENSE,  "Material D"));
  _upgradematerials.push_back(new UpgradeMaterial(UpgradeMaterial::Type::PHYSICAL, "Material P"));
  if(Settings->GetBool("cheat_cheap_upgrades"))
    for(auto u : _upgradematerials)
      u->Add(9999);
}


void GameStats::Tick(double deltatime)
{
  _time += deltatime;
  if(_score_multiplier_timer > 0.0)
    _score_multiplier_timer -= deltatime;
}


void GameStats::AddScore(unsigned int amount)
{
  if(_score_multiplier_timer > 0.0)
    amount *= _score_multiplier;

  _score += amount;
}


void GameStats::SetScoreMultiplier(unsigned int multiplier, double timer)
{
  _score_multiplier = multiplier;
  _score_multiplier_timer = timer;
}


unsigned int GameStats::GetScore() const
{
  return _score;
}


unsigned int GameStats::GetScoreMultiplier() const
{
  if(_score_multiplier_timer > 0.0)
    return _score_multiplier;
  return 1;
}


UpgradeMaterial * GameStats::GetUpgradeMaterial(UpgradeMaterial::Type type)
{
  for(auto u : _upgradematerials)
    if(u->GetType() == type)
      return u;
  
  assert(false);
  return nullptr;
}


std::vector<UpgradeMaterial *> & GameStats::GetUpgradeMaterials()
{
  return _upgradematerials;
}


int GameStats::GetLives() const
{
  return _lives;
}


void GameStats::AdjustLives(int amount)
{
  _lives += amount;
}


double GameStats::GetTime() const
{
  return _time;
}


void GameStats::OnHumanCollected()
{
  _total_humans_collected++;
}


void GameStats::OnHumansSaved(unsigned int count)
{
  _total_humans_saved += count;
}


unsigned int GameStats::GetHumansCollected() const
{
  return _total_humans_collected;
}


unsigned int GameStats::GetHumansSaved() const
{
  return _total_humans_saved;
}


std::pair<bool, SpaceshipUpgrade::Type> GameStats::GetRandomUnlockableSpaceshipUpgrade(float random)
{
  std::vector<SpaceshipUpgrade::Type> all
    {
      SpaceshipUpgrade::Type::WEAPON,
      SpaceshipUpgrade::Type::WEAPON_COOLER,
      SpaceshipUpgrade::Type::ENGINE_UPGRADE,
      SpaceshipUpgrade::Type::HULL_UPGRADE,
      SpaceshipUpgrade::Type::REPAIR_DROID,
      SpaceshipUpgrade::Type::WARP_ENGINE,
      SpaceshipUpgrade::Type::PLANET_LANDER,
    };
  std::vector<SpaceshipUpgrade::Type> possible;
  for(auto u : all)
    if(!IsSpaceshipUpgradeAvailable(u))
      possible.push_back(u);
  if(possible.size() == 0)
    return std::make_pair(false, SpaceshipUpgrade::Type::BONUS_DAMAGE);

  if(possible.size() == all.size())
    return std::make_pair(true, SpaceshipUpgrade::Type::PLANET_LANDER);
  
  auto r = static_cast<unsigned int>(random * static_cast<float>(possible.size()));
  r = std::clamp(r, 0u, static_cast<unsigned int>(possible.size() - 1));

  return std::make_pair(true, possible[r]);
}

void GameStats::UnlockSpaceshipUpgrade(SpaceshipUpgrade::Type type)
{
  assert(_blueprint_points > 0);
  _blueprint_points--;
  
  _spaceship_upgrade_blueprints[type] = true;
}


bool GameStats::IsSpaceshipUpgradeAvailable(SpaceshipUpgrade::Type type)
{
  auto it = _spaceship_upgrade_blueprints.find(type);
  if(it != _spaceship_upgrade_blueprints.end())
    return (*it).second;
  else
    return false;
}


void GameStats::AddBlueprintPoints(unsigned int amount)
{
  _blueprint_points += amount;
}


unsigned int GameStats::GetBlueprintPoints() const
{
  return _blueprint_points;
}

