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
      EVASION_MANEUVER,
    };
  
  SpaceshipUpgrade(ObjectSpaceship * spaceship, Type type);

  bool         CanBeInstalled() const;
  unsigned int GetNextPurchaseCost(UpgradeMaterial::Type for_material) const;
  int          GetInstallCount() const;
  int          GetMaxInstalls()  const;
  void         Install();
  
  void Tick(double deltatime);

  Type   GetType()         const;
  double GetValue()        const;
  void   AdjustValue(double amount);
  double GetTimer()        const;
  const std::string & GetName() const;

  bool CanActivate() const;
  void Activate(double time);
  void Activate(double value, double time);
  void ActivateFromCollectible(ObjectCollectible * collectible);
  bool IsActive() const;

  void Dump() const;
  
private:
  ObjectSpaceship * _spaceship;
  Type        _type;
  std::string _name;
  double      _value;
  int         _install_count;
  bool        _always_activated;
  bool        _activated;
  double      _timer;
  double      _cooldown;
};

#endif
