#ifndef SPACESHIP_UPGRADE_HH_
#define SPACESHIP_UPGRADE_HH_
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

class ObjectCollectible;
class ObjectSpaceship;


class SpaceshipUpgrade
{
public:
  enum class Type
    {
      BONUS_DAMAGE,
      SHIELD,
      WEAPON,
      WEAPON_COOLER,
      ENGINE_UPGRADE,
      HULL_UPGRADE,
      REPAIR_DROID,
      WARP_ENGINE,
      PLANET_LANDER,
      BOMB_BAY,
    };
  
  SpaceshipUpgrade(ObjectSpaceship * spaceship, Type type);

  void         SetOwner(ObjectSpaceship * spaceship);
  bool         CanBeInstalled() const;
  unsigned int GetNextPurchaseCost(UpgradeMaterial::Type for_material) const;
  int          GetInstallCount() const;
  int          GetMaxInstalls()  const;
  void         Install();
  
  void Tick(double deltatime);

  Type   GetType()         const;
  double GetValue()        const;
  double GetRawValue()     const;
  void   AdjustValue(double amount);
  double GetTimer()        const;
  double GetTimerMax()     const;
  const std::string & GetName() const;
  double GetCooldownRemaining() const;
  double GetCooldownMax() const;

  bool CanActivate() const;
  void Activate();
  void Activate(double time);
  void Activate(double value, double time);
  void ActivateFromCollectible(ObjectCollectible * collectible);
  void Deactivate();
  bool IsActive() const;
  
  void SetEnabled(bool enabled);

  void Dump() const;
  
private:
  ObjectSpaceship * _spaceship;
  Type        _type;
  std::string _name;
  double      _value;
  int         _install_count;
  bool        _enabled;
  bool        _always_activated;
  bool        _activated;
  bool        _uses_timer;
  double      _timer;
  double      _timer_max;
  double      _cooldown_default;
  double      _cooldown;
};

#endif
