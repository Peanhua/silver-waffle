#ifndef UPGRADE_MATERIAL_HH_
#define UPGRADE_MATERIAL_HH_

#include <string>
#include <cassert>

class UpgradeMaterial
{
public:
  enum class Type
    {
      ATTACK,
      DEFENSE,
      PHYSICAL
    };
  
  UpgradeMaterial(Type type, const std::string & name)
    : _type(type),
      _name(name),
      _amount(0)
  {
  }


  Type GetType() const
  {
    return _type;
  }

  
  const std::string GetName() const
  {
    return _name;
  }

  
  unsigned int GetAmount() const
  {
    return _amount;
  }


  void Add(unsigned int amount)
  {
    assert(_amount + amount > _amount);
    _amount += amount;
  }

  
  void Use(unsigned int amount)
  {
    assert(_amount >= amount);
    _amount -= amount;
  }
  
  
private:
  Type         _type;
  std::string  _name;
  unsigned int _amount;
};

#endif
