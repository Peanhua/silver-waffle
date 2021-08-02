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
#include "SpaceshipUpgrade.hh"
#include <vector>

class ObjectBuilding;
class ObjectCollectible;
class ObjectPlanet;
class GameStats;
class SpaceshipControlProgram;


class ObjectSpaceship : public ObjectMovable
{
public:
  typedef std::function<void()> on_human_count_changed_t;
  
  ObjectSpaceship(Scene * scene, unsigned int random_seed);

  void Tick(double deltatime) override;
  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;
  uint64_t GetCollidesWithChannels() const override;
  double GetGlow() const override;
  
  void         RemoveWeapons();
  void         AddWeapon();
  void         AddWeapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage);
  unsigned int GetWeaponCount() const;
  double       GetWeaponHeat(unsigned int weapon_id) const;
  bool         FireWeapon(unsigned int weapon_id);
  void         SetWeaponAutofire(unsigned int weapon_id, bool enabled);
  void         EnableWeapons(bool enabled);
  
  void         AddEngine(const glm::vec3 & thrust_direction, double power);
  unsigned int GetEngineCount() const;
  void         SetEngineThrottle(unsigned int engine_id, double throttle);
  void         UpgradeEngines(double power_multiplier);
  void         EnableEngine(unsigned int engine_id, bool enabled);
  void         EnableEngines(bool enabled);

  void               UpgradeFromCollectible(ObjectCollectible * collectible);
  SpaceshipUpgrade * GetUpgrade(SpaceshipUpgrade::Type type) const;

  void        SetOwnerGameStats(GameStats * gamestats);
  GameStats * GetOwnerGameStats() const;

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
  void DescendToPlanet(ObjectPlanet * planet);
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

  class Weapon
  {
  public:
    Weapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage);
    
    glm::vec3 _location;
    bool      _autofire;
    double    _heat;
    double    _last_fire_timer;
    double    _minimum_firing_interval;
    Mesh *    _projectile;
    glm::vec3 _projectile_direction;
    double    _projectile_initial_velocity;
    double    _projectile_damage;
  private:
  };

  GameStats *            _gamestats;
  bool                   _landed;
  std::vector<Engine *>  _engines;
  std::vector<Weapon *>  _weapons;
  std::vector<SpaceshipUpgrade *> _upgrades;
  std::vector<SpaceshipControlProgram *> _control_programs;
  int                    _human_count;
  on_human_count_changed_t _on_human_count_changed;
  double                 _human_saving_timer;
  bool        _systemlog_enabled;
  std::string _systemlog;
};

#endif
