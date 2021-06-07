#ifndef SPACESHIP_UPGRADE_HH_
#define SPACESHIP_UPGRADE_HH_

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
    };
  
  SpaceshipUpgrade(ObjectSpaceship * spaceship, Type type);

  bool CanAdd() const;
  void Add(double amount, double time);
  void Tick(double deltatime);
  void AddFromCollectible(ObjectCollectible * collectible);

  bool   IsActive()    const;
  Type   GetType()     const;
  double GetValue()    const;
  int    GetIntValue() const;
  double GetTimer()    const;
  unsigned int GetNextPurchaseCost(UpgradeMaterial::Type for_material) const;
  const std::string & GetName() const;
    
private:
  ObjectSpaceship * _spaceship;
  Type        _type;
  std::string _name;
  double      _value;
  int         _int_value;
  double      _timer;
};

#endif
