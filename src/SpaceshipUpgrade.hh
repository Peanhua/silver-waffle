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
      REPAIR_DROID,
      WARP_ENGINE,
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
  double      _timer_max;
  double      _cooldown;
};

#endif
