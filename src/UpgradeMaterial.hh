#ifndef UPGRADE_MATERIAL_HH_
#define UPGRADE_MATERIAL_HH_

#include "Observer.hh"
#include <string>


class UpgradeMaterial : public Subject<UpgradeMaterial>
{
public:
  enum class Type
    {
      ATTACK,
      DEFENSE,
      PHYSICAL
    };
  
  UpgradeMaterial(Type type, const std::string & name);

  Type              GetType()   const;
  const std::string GetName()   const;
  unsigned int      GetAmount() const;

  void Add(unsigned int amount);
  void Use(unsigned int amount);
  
  
private:
  Type         _type;
  std::string  _name;
  unsigned int _amount;
};

#endif
