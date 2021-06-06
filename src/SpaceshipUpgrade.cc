#include "SpaceshipUpgrade.hh"
#include "ObjectCollectible.hh"
#include "ObjectSpaceship.hh"
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
    case Type::BONUS_DAMAGE:  _name = "Bonus damage";  break;
    case Type::SHIELD:        _name = "Shield";        break;
    case Type::WEAPON:        _name = "Weapon";        break;
    case Type::WEAPON_COOLER: _name = "Weapon cooler"; break;
    }
}


bool SpaceshipUpgrade::CanAdd() const
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
    case Type::SHIELD:
      return true;

    case Type::WEAPON:
      return _spaceship->GetWeaponCount() < 5;

    case Type::WEAPON_COOLER:
      return _spaceship->GetUpgrade(Type::WEAPON_COOLER)->GetIntValue() < static_cast<int>(_spaceship->GetWeaponCount());
    }
  assert(false);
  return false;
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



double SpaceshipUpgrade::GetTimer() const
{
  return _timer;
}


unsigned int SpaceshipUpgrade::GetNextPurchaseCost(UpgradeMaterial::Type for_material) const
{
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
    case Type::WEAPON:
      break;
    case Type::WEAPON_COOLER:
      break;
    }
}


const std::string & SpaceshipUpgrade::GetName() const
{
  return _name;
}
