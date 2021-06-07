#include "UpgradeMaterial.hh"
#include <cassert>


UpgradeMaterial::UpgradeMaterial(Type type, const std::string & name)
  : _type(type),
    _name(name),
    _amount(0)
{
  switch(type)
    {
    case Type::ATTACK:   _name = "A"; break;
    case Type::DEFENSE:  _name = "D"; break;
    case Type::PHYSICAL: _name = "P"; break;
    }
}


UpgradeMaterial::Type UpgradeMaterial::GetType() const
{
  return _type;
}

  
const std::string UpgradeMaterial::GetName() const
{
  return _name;
}

  
unsigned int UpgradeMaterial::GetAmount() const
{
  return _amount;
}


void UpgradeMaterial::Add(unsigned int amount)
{
  assert(_amount + amount > _amount);
  _amount += amount;

  NotifyObservers();
}

  
void UpgradeMaterial::Use(unsigned int amount)
{
  assert(_amount >= amount);
  _amount -= amount;

  NotifyObservers();
}
