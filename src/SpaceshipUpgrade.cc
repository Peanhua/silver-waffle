#include "SpaceshipUpgrade.hh"
#include "ObjectCollectible.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemSettings.hh"
#include <map>


SpaceshipUpgrade::SpaceshipUpgrade(ObjectSpaceship * spaceship, Type type)
  : _spaceship(spaceship),
    _type(type),
    _value(0),
    _int_value(0),
    _timer(0)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:   _name = "Bonus damage";   break;
    case Type::SHIELD:         _name = "Shield";         break;
    case Type::WEAPON:         _name = "Weapon";         break;
    case Type::WEAPON_COOLER:  _name = "Weapon cooler";  break;
    case Type::ENGINE_UPGRADE: _name = "Engine upgrade"; break;
    case Type::HULL_UPGRADE:   _name = "Hull upgrade";   break;
    }
}


bool SpaceshipUpgrade::CanAdd() const
{
  return GetIntValue() < GetMaxIntValue();
}



void SpaceshipUpgrade::Add(double amount, double time)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      _value = amount;
      _timer = time;
      break;
    case Type::SHIELD:
      _value = amount;
      _timer += time;
      break;
    case Type::WEAPON:
      _spaceship->AddWeapon();
      //_int_value = static_cast<int>(_spaceship->GetWeaponCount()); handled in getter
      break;
    case Type::WEAPON_COOLER:
      _int_value++;
      break;
    case Type::ENGINE_UPGRADE:
      _int_value++;
      _spaceship->UpgradeEngines(1.5);
      break;
    case Type::HULL_UPGRADE:
      {
        _int_value++;
        double hp = _spaceship->GetHealth() / _spaceship->GetMaxHealth();
        _spaceship->SetMaxHealth(_spaceship->GetMaxHealth() * 2.0);
        _spaceship->SetHealth(hp * _spaceship->GetMaxHealth());
      }
      break;
    }
  if(_value < 0.0001)
    _timer = 0;
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


int SpaceshipUpgrade::GetIntValue() const
{
  if(!IsActive())
    return 0;

  if(_type == Type::WEAPON)
    return static_cast<int>(_spaceship->GetWeaponCount());
  
  return _int_value;
}



int SpaceshipUpgrade::GetMaxIntValue() const
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      return 1;
    case Type::SHIELD:
      return 1;
    case Type::WEAPON:
      return 5;
    case Type::WEAPON_COOLER:
      return static_cast<int>(_spaceship->GetWeaponCount());
    case Type::ENGINE_UPGRADE:
      return 3;
    case Type::HULL_UPGRADE:
      return 1;
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
    }
  return costs[for_material];
}


void SpaceshipUpgrade::Tick(double deltatime)
{
  if(IsActive() && _timer > 0.0)
    _timer -= deltatime;
}


bool SpaceshipUpgrade::IsActive() const
{
  bool uses_timer;
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
    case Type::SHIELD:
      uses_timer = true;
      break;
    case Type::WEAPON:
    case Type::WEAPON_COOLER:
    case Type::ENGINE_UPGRADE:
    case Type::HULL_UPGRADE:
      uses_timer = false;
      break;
    }

  if(uses_timer)
    return _timer > 0.0;
  else
    return true;
}


void SpaceshipUpgrade::AddFromCollectible(ObjectCollectible * collectible)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      if(collectible->HasBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER))
        Add(collectible->GetBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER), 30.0);
      break;
    case Type::SHIELD:
      if(collectible->HasBonus(ObjectCollectible::TYPE_SHIELD))
        Add(std::max(_value, collectible->GetBonus(ObjectCollectible::TYPE_SHIELD)), 30.0);
      break;
      // The Following are not available from ObjectCollectible:
    case Type::WEAPON:
    case Type::WEAPON_COOLER:
    case Type::ENGINE_UPGRADE:
    case Type::HULL_UPGRADE:
      break;
    }
}


const std::string & SpaceshipUpgrade::GetName() const
{
  return _name;
}
