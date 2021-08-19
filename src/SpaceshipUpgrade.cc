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
#include "Mesh.hh"
#include "ObjectBuilding.hh"
#include "ObjectCollectible.hh"
#include "ObjectSpaceship.hh"
#include "QuadTree.hh"
#include "Scene.hh"
#include "SpaceshipControlProgram.hh"
#include "SubsystemSettings.hh"
#include <algorithm>
#include <map>
#include <iostream>


SpaceshipUpgrade::SpaceshipUpgrade(ObjectSpaceship * spaceship, Type type)
  : _spaceship(spaceship),
    _type(type),
    _value(0),
    _install_count(0),
    _enabled(true),
    _activated(false),
    _timer(0),
    _timer_max(0),
    _cooldown_default(30),
    _cooldown(0)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:     _name = "Bonus damage";   _always_activated = false; _uses_timer = true;  _install_count = 1; break;
    case Type::SHIELD:           _name = "Shield";         _always_activated = true;  _uses_timer = false; _install_count = 1; break;
    case Type::WEAPON:           _name = "Weapon";         _always_activated = true;  _uses_timer = false; break;
    case Type::WEAPON_COOLER:    _name = "Weapon cooler";  _always_activated = true;  _uses_timer = false; break;
    case Type::ENGINE_UPGRADE:   _name = "Engine upgrade"; _always_activated = true;  _uses_timer = false; break;
    case Type::HULL_UPGRADE:     _name = "Hull upgrade";   _always_activated = true;  _uses_timer = false; break;
    case Type::REPAIR_DROID:     _name = "Repair droid";   _always_activated = true;  _uses_timer = false; break;
    case Type::WARP_ENGINE:      _name = "Warp engine";    _always_activated = false; _uses_timer = true;  _cooldown_default = 0; break;
    case Type::PLANET_LANDER:    _name = "Planet lander";  _always_activated = false; _uses_timer = false; _cooldown_default = 1; break;
    }
}



void SpaceshipUpgrade::SetOwner(ObjectSpaceship * spaceship)
{
  _spaceship = spaceship;
}


bool SpaceshipUpgrade::CanBeInstalled() const
{
  return GetInstallCount() < GetMaxInstalls();
}


void SpaceshipUpgrade::Install()
{
  _install_count++;
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
    case Type::SHIELD:
      assert(false);
      break;
    case Type::WEAPON:
      _spaceship->AddWeapon();
      break;
    case Type::WEAPON_COOLER:
      break;
    case Type::ENGINE_UPGRADE:
      _spaceship->UpgradeEngines(1.6);
      break;
    case Type::HULL_UPGRADE:
      {
        double hp = _spaceship->GetHealth() / _spaceship->GetMaxHealth();
        _spaceship->SetMaxHealth(_spaceship->GetMaxHealth() * 2.0);
        _spaceship->SetHealth(hp * _spaceship->GetMaxHealth());
      }
      break;
    case Type::REPAIR_DROID:
      break;
    case Type::WARP_ENGINE:
      _value = 10.0;
      _timer += 30.0;
      _timer_max += 5.0 * 60.0;
      break;
    case Type::PLANET_LANDER:
      break;
    }
  _spaceship->SystemlogAppend("Installed: " + _name + "\n");
}


SpaceshipUpgrade::Type SpaceshipUpgrade::GetType() const
{
  return _type;
}


double SpaceshipUpgrade::GetValue() const
{
  if(!IsActive())
    return 0.0;

  return _value;
}

double SpaceshipUpgrade::GetRawValue() const
{
  return _value;
}

int SpaceshipUpgrade::GetInstallCount() const
{
  if(_type == Type::WEAPON)
    return static_cast<int>(_spaceship->GetWeaponCount());
  
  return _install_count;
}



int SpaceshipUpgrade::GetMaxInstalls() const
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:     return 1;
    case Type::SHIELD:           return 1;
    case Type::WEAPON:           return 5;
    case Type::WEAPON_COOLER:    return static_cast<int>(_spaceship->GetWeaponCount());
    case Type::ENGINE_UPGRADE:   return 3;
    case Type::HULL_UPGRADE:     return 1;
    case Type::REPAIR_DROID:     return 1;
    case Type::WARP_ENGINE:      return 1;
    case Type::PLANET_LANDER:    return 1;
    }
  assert(false);
  return 0;
}


double SpaceshipUpgrade::GetTimer() const
{
  return _timer;
}


double SpaceshipUpgrade::GetTimerMax() const
{
  return _timer_max;
}


unsigned int SpaceshipUpgrade::GetNextPurchaseCost(UpgradeMaterial::Type for_material) const
{
  if(Settings->GetBool("cheat_cheap_upgrades"))
    {
      if(for_material == UpgradeMaterial::Type::ATTACK)
        return 1;
      else
        return 0;
    }
  
  std::map<UpgradeMaterial::Type, unsigned int> costs;
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
    case Type::SHIELD:
      costs[UpgradeMaterial::Type::ATTACK]   =  0;
      costs[UpgradeMaterial::Type::DEFENSE]  =  0;
      costs[UpgradeMaterial::Type::PHYSICAL] =  0;
      break;
    case Type::WEAPON:
      costs[UpgradeMaterial::Type::ATTACK]   = 10;
      costs[UpgradeMaterial::Type::DEFENSE]  =  0;
      costs[UpgradeMaterial::Type::PHYSICAL] = 10;
      break;
    case Type::WEAPON_COOLER:
      costs[UpgradeMaterial::Type::ATTACK]   =  6;
      costs[UpgradeMaterial::Type::DEFENSE]  = 10;
      costs[UpgradeMaterial::Type::PHYSICAL] =  4;
      break;
    case Type::ENGINE_UPGRADE:
      costs[UpgradeMaterial::Type::ATTACK]   =  5;
      costs[UpgradeMaterial::Type::DEFENSE]  =  5;
      costs[UpgradeMaterial::Type::PHYSICAL] =  2;
      break;
    case Type::HULL_UPGRADE:
      costs[UpgradeMaterial::Type::ATTACK]   =  0;
      costs[UpgradeMaterial::Type::DEFENSE]  = 10;
      costs[UpgradeMaterial::Type::PHYSICAL] = 20;
      break;
    case Type::REPAIR_DROID:
      costs[UpgradeMaterial::Type::ATTACK]   = 10;
      costs[UpgradeMaterial::Type::DEFENSE]  = 10;
      costs[UpgradeMaterial::Type::PHYSICAL] = 10;
      break;
    case Type::WARP_ENGINE:
      costs[UpgradeMaterial::Type::ATTACK]   =  1;
      costs[UpgradeMaterial::Type::DEFENSE]  =  1;
      costs[UpgradeMaterial::Type::PHYSICAL] =  2;
      break;
    case Type::PLANET_LANDER:
      costs[UpgradeMaterial::Type::ATTACK]   =  1;
      costs[UpgradeMaterial::Type::DEFENSE]  =  1;
      costs[UpgradeMaterial::Type::PHYSICAL] =  3;
      break;
    }
  return costs[for_material];
}


void SpaceshipUpgrade::Tick(double deltatime)
{
  assert(_spaceship->IsAlive());
  
  if(IsActive())
    {
      if(_uses_timer)
        {
          if(_timer > 0.0)
            _timer -= deltatime;
          else
            Deactivate();
        }

      switch(_type)
        {
        case Type::BONUS_DAMAGE:
        case Type::SHIELD:
        case Type::WEAPON:
        case Type::WEAPON_COOLER:
        case Type::ENGINE_UPGRADE:
        case Type::HULL_UPGRADE:
        case Type::WARP_ENGINE:
        case Type::PLANET_LANDER:
          break;
          
        case Type::REPAIR_DROID:
          {
            double repair = std::clamp(_spaceship->GetMaxHealth() - _spaceship->GetHealth(), 0.0, 3.0);
            if(repair > 0.0)
              _spaceship->SetHealth(_spaceship->GetHealth() + repair * deltatime);
          }
          break;
          
        }
    }
  else
    {
      if(_cooldown > 0.0)
        _cooldown -= deltatime;
    }

  if(_spaceship->IsLanded())
    _spaceship->SetHealth(_spaceship->GetHealth() + 25.0 * deltatime);
}


bool SpaceshipUpgrade::IsActive() const
{
  return GetInstallCount() > 0 && (_always_activated || _activated);
}


void SpaceshipUpgrade::ActivateFromCollectible(ObjectCollectible * collectible)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      if(collectible->HasBonus(ObjectCollectible::Type::DAMAGE_MULTIPLIER))
        {
          _value = collectible->GetBonus(ObjectCollectible::Type::DAMAGE_MULTIPLIER);
          _timer = 10;
          _spaceship->SystemlogAppend("Damage boost liquid.\n");
          if(GetOwnerGameStats())
            GetScene()->TutorialMessage(3, "Press F to activate.\n");
        }
      break;
    case Type::SHIELD:
      if(collectible->HasBonus(ObjectCollectible::Type::SHIELD))
        Activate(std::clamp(std::max(_value, collectible->GetBonus(ObjectCollectible::Type::SHIELD)), 0.0, _spaceship->GetMaxHealth()),
                 0.0);
      break;
    case Type::WARP_ENGINE:
      if(GetInstallCount() > 0)
        if(collectible->HasBonus(ObjectCollectible::Type::WARP_FUEL))
          {
            auto prev = _timer;
            _timer += collectible->GetBonus(ObjectCollectible::Type::WARP_FUEL);
            if(_timer > _timer_max)
              _timer = _timer_max;
            _spaceship->SystemlogAppend("Warp fuel +" + std::to_string(static_cast<int>(_timer - prev)) + "\n");
          }
      break;
      // The Following are not available from ObjectCollectible:
    case Type::WEAPON:
    case Type::WEAPON_COOLER:
    case Type::ENGINE_UPGRADE:
    case Type::HULL_UPGRADE:
    case Type::REPAIR_DROID:
    case Type::PLANET_LANDER:
      break;
    }
}


const std::string & SpaceshipUpgrade::GetName() const
{
  return _name;
}


bool SpaceshipUpgrade::CanActivate() const
{
  if(GetInstallCount() == 0)
    return false;
  
  if(IsActive())
    return false;

  if(!_enabled)
    return false;

  if(_cooldown > 0.0)
    return false;

  if(_type == Type::WARP_ENGINE)
    {
      if(_timer < 0.01)
        return false;
      if(!_spaceship->GetScene()->CanUseWarpEngine())
        return false;
    }

  return true;
}


void SpaceshipUpgrade::Activate(double value, double time)
{
  assert(_enabled);
  _activated = true;
  _value = value;
  _spaceship->SystemlogAppend(_name + " activated.\n");
  _cooldown = _cooldown_default;
  if(_type == Type::WARP_ENGINE)
    {
    }
  else
    {
      _timer = time;
      _timer_max = time;
    }

  if(_type == Type::PLANET_LANDER)
    { // Launch from spaceport, or Land on spaceport.
      Deactivate();

      auto playerpos = _spaceship->GetPosition();
      auto scene = _spaceship->GetScene();

      if(_spaceship->IsLanded())
        {
          _spaceship->LaunchFromSpaceport();
        }
      else
        {
          bool found = false;
          auto spaceport = scene->GetClosestSpaceport(playerpos);
          if(spaceport)
            if(glm::distance(playerpos, spaceport->GetPosition()) < 5)
              {
                found = true;
                _spaceship->LandOnSpaceport(spaceport);
              }

          if(!found)
            _spaceship->SystemlogAppend(_name + ": Error, no spaceport nearby.\n");
        }
    }
}

void SpaceshipUpgrade::Activate(double time)
{
  Activate(_value, time);
}

void SpaceshipUpgrade::Activate()
{
  Activate(_value, _timer);
}



void SpaceshipUpgrade::AdjustValue(double amount)
{
  _value += amount;
  if(_value < 0.00001)
    _activated = false;
}


void SpaceshipUpgrade::Dump() const
{
  std::cout << this << ": SpaceshipUpgrade: ";
  std::cout << ", _spaceship=" << _spaceship;
  std::cout << ", _type=" << static_cast<int>(_type);
  std::cout << ", _name=" << _name;
  std::cout << ", _value=" << _value;
  std::cout << ", _install_count=" << _install_count;
  std::cout << ", _always_activated=" << _always_activated;
  std::cout << ", _activated=" << _activated;
  std::cout << ", _timer=" << _timer;
  std::cout << ", _timer_max=" << _timer_max;
  std::cout << ", _cooldown=" << _cooldown;
  std::cout << std::endl;
}


double SpaceshipUpgrade::GetCooldownRemaining() const
{
  return _cooldown;
}


double SpaceshipUpgrade::GetCooldownMax() const
{
  return 30.0;
}


void SpaceshipUpgrade::Deactivate()
{
  if(_activated)
    {
      if(_type == Type::WARP_ENGINE)
        _spaceship->SystemlogAppend("Warp engine: off\n");
      else if(_type == Type::BONUS_DAMAGE)
        _value = 0;
    }
  _activated = false;
}


void SpaceshipUpgrade::SetEnabled(bool enabled)
{
  _spaceship->SystemlogAppend(_name + (enabled ? " online" : " offline") + "\n");
  _enabled = enabled;
}
