#ifndef OBJECT_SPACESHIP_HH_
#define OBJECT_SPACESHIP_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "ObjectMovable.hh"
#include "RingBuffer.hh"
#include "SpaceshipUpgrade.hh"
#include <vector>

class ObjectBuilding;
class ObjectCollectible;
class ObjectProjectile;
class SolarSystemObject;
class SpaceshipControlProgram;
class Weapon;


class ObjectSpaceship : public ObjectMovable
{
public:
  typedef std::function<void()> on_human_count_changed_t;
  
  ObjectSpaceship(Scene * scene, unsigned int random_seed);

  void     Tick(double deltatime) override;
  void     OnHit(Object * perpetrator, Object * hitter, double damage, const glm::vec3 & hit_position, const glm::vec3 & impulse) override;
  double   GetGlow() const override;
  void     OnDestroyed(Object * destroyer) override;
  
  void         RemoveWeapons();
  void         AddWeapon();
  void         AddWeaponBomb();
  unsigned int GetWeaponCount(unsigned int weapon_group) const;
  unsigned int GetWeaponGroupCount() const;
  void         SetWeaponGroupAutofire(unsigned int weapon_group, bool enabled);
  void         EnableWeapons(bool enabled);
  void         OnFiredProjectileDestroyed(ObjectProjectile * projectile);
  const std::vector<Weapon *> & GetWeapons(unsigned int weapon_group) const;
  
  void         AddEngine(const glm::vec3 & thrust_direction, double power);
  unsigned int GetEngineCount() const;
  void         SetEngineThrottle(unsigned int engine_id, double throttle);
  double       GetEngineThrottle(unsigned int engine_id);
  void         UpgradeEngines(double power_multiplier);
  void         EnableEngine(unsigned int engine_id, bool enabled);
  void         EnableEngines(bool enabled);
  void         SetEngineSound(bool enabled);
  void         SetEngineAutoSlowDownAxis(int axis, bool enabled);
  
  void               UpgradeFromCollectible(ObjectCollectible * collectible);
  SpaceshipUpgrade * GetUpgrade(SpaceshipUpgrade::Type type) const;

  void CopyUpgrades(const ObjectSpaceship & source);

  void         ClearControlPrograms();
  void         AddControlProgram(SpaceshipControlProgram * program);
  void         AddNamedControlProgram(const std::string & name);
  unsigned int GetActiveControlProgramCount() const;

  void                SystemlogEnable();
  void                SystemlogAppend(const std::string & message);
  const std::string & SystemlogGet() const;
  void                SystemlogClear();

  void AddHuman();
  int  GetHumanCount() const;
  void SaveHuman(double deltatime);

  void SetOnHumanCountChanged(on_human_count_changed_t callback);

  void LandOnSpaceport(ObjectBuilding * spaceport);
  void LaunchFromSpaceport();
  void DescendToPlanet(SolarSystemObject * planet);
  void LaunchToSpace();

  bool IsLanded() const;
  
private:
  class Engine
  {
  public:
    glm::vec3 _thrust_direction;
    double    _power;
    double    _throttle;
    bool      _enabled;
    float     _speed_limit;
  private:
  };

  bool                   _landed;
  std::vector<Engine *>  _engines;
  std::vector<std::vector<Weapon *>>  _weapongroups;
  RingBuffer<ObjectProjectile *> _projectiles_fired;
  std::vector<SpaceshipUpgrade *> _upgrades;
  std::vector<SpaceshipControlProgram *> _control_programs;
  int                    _human_count;
  on_human_count_changed_t _on_human_count_changed;
  double                 _human_saving_timer;
  bool        _systemlog_enabled;
  std::string _systemlog;
  bool        _has_engine_sound;
  double      _engine_sfx_timer;
  int         _engine_auto_slow_down_axis[3];

  Weapon *     AddWeapon(unsigned int weapon_group, const glm::vec3 & location, const glm::vec3 & projectile_direction);
};

#endif
