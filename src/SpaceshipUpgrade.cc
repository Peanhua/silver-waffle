#include "SpaceshipUpgrade.hh"
#include "ObjectCollectible.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemSettings.hh"
#include <map>
#include <iostream>


SpaceshipUpgrade::SpaceshipUpgrade(ObjectSpaceship * spaceship, Type type)
  : _spaceship(spaceship),
    _type(type),
    _value(0),
    _install_count(0),
    _activated(false),
    _timer(0),
    _cooldown(0)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:     _name = "Bonus damage";   _always_activated = false; break;
    case Type::SHIELD:           _name = "Shield";         _always_activated = false; break;
    case Type::WEAPON:           _name = "Weapon";         _always_activated = true;  break;
    case Type::WEAPON_COOLER:    _name = "Weapon cooler";  _always_activated = true;  break;
    case Type::ENGINE_UPGRADE:   _name = "Engine upgrade"; _always_activated = true;  break;
    case Type::HULL_UPGRADE:     _name = "Hull upgrade";   _always_activated = true;  break;
    case Type::EVASION_MANEUVER: _name = "Evasion CPU";    _always_activated = false; break;
    }
}


bool SpaceshipUpgrade::CanBeInstalled() const
{
  return GetInstallCount() < GetMaxInstalls();
}


void SpaceshipUpgrade::Install()
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
    case Type::SHIELD:
      assert(false);
      break;
    case Type::WEAPON:
      _spaceship->AddWeapon();
      //_install_count = static_cast<int>(_spaceship->GetWeaponCount()); handled in getter
      break;
    case Type::WEAPON_COOLER:
      _install_count++;
      break;
    case Type::ENGINE_UPGRADE:
      _install_count++;
      _spaceship->UpgradeEngines(1.5);
      break;
    case Type::HULL_UPGRADE:
      {
        _install_count++;
        double hp = _spaceship->GetHealth() / _spaceship->GetMaxHealth();
        _spaceship->SetMaxHealth(_spaceship->GetMaxHealth() * 2.0);
        _spaceship->SetHealth(hp * _spaceship->GetMaxHealth());
      }
      break;
    case Type::EVASION_MANEUVER:
      _install_count++;
      break;
    }
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
    case Type::EVASION_MANEUVER: return 1;
    }
  assert(false);
  return 0;
}


double SpaceshipUpgrade::GetTimer() const
{
  return _timer;
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
    case Type::EVASION_MANEUVER:
      costs[UpgradeMaterial::Type::ATTACK]   =  5;
      costs[UpgradeMaterial::Type::DEFENSE]  =  0;
      costs[UpgradeMaterial::Type::PHYSICAL] = 10;
      break;
    }
  return costs[for_material];
}


void SpaceshipUpgrade::Tick(double deltatime)
{
  if(IsActive())
    {
      if(_timer > 0.0)
        {
          _timer -= deltatime;
          
          if(_type == Type::EVASION_MANEUVER)
            {
              if(_timer > 0.0)
                _spaceship->RotateRoll(1200 * deltatime);
              else
                {
                  auto o = glm::angleAxis(0.0f, _spaceship->GetForwardVector());
                  _spaceship->SetOrientation(o);
                }
            }
        }
      else
        {
          _activated = false;
        }
    }

  if(_cooldown > 0.0)
    _cooldown -= deltatime;
}


bool SpaceshipUpgrade::IsActive() const
{
  return _always_activated || _activated;
}


void SpaceshipUpgrade::ActivateFromCollectible(ObjectCollectible * collectible)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      if(collectible->HasBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER))
        Activate(collectible->GetBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER), 30.0);
      break;
    case Type::SHIELD:
      if(collectible->HasBonus(ObjectCollectible::TYPE_SHIELD))
        Activate(std::max(_value, collectible->GetBonus(ObjectCollectible::TYPE_SHIELD)), 30.0);
      break;
      // The Following are not available from ObjectCollectible:
    case Type::WEAPON:
    case Type::WEAPON_COOLER:
    case Type::ENGINE_UPGRADE:
    case Type::HULL_UPGRADE:
    case Type::EVASION_MANEUVER:
      break;
    }
}


const std::string & SpaceshipUpgrade::GetName() const
{
  return _name;
}


bool SpaceshipUpgrade::CanActivate() const
{
  if(IsActive())
    return false;

  if(_cooldown > 0.0)
    return false;

  return true;
}


void SpaceshipUpgrade::Activate(double value, double time)
{
  _activated = true;
  _value = value;
  _timer = time;
  _cooldown = 30.0;
}

void SpaceshipUpgrade::Activate(double time)
{
  Activate(_value, time);
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
  std::cout << ", _cooldown=" << _cooldown;
  std::cout << std::endl;
}


