#ifndef OBJECT_SPACESHIP_HH_
#define OBJECT_SPACESHIP_HH_

#include "ObjectMovable.hh"
#include "SpaceshipUpgrade.hh"
#include <vector>

class ObjectCollectible;
class GameStats;
class SpaceshipControlProgram;


class ObjectSpaceship : public ObjectMovable
{
public:
  ObjectSpaceship(Scene * scene);

  void Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const override;
  void Tick(double deltatime) override;
  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;
  uint64_t GetCollidesWithChannels() const override;

  unsigned int AddWeapon();
  unsigned int AddWeapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage);
  unsigned int GetWeaponCount() const;
  double       GetWeaponHeat(unsigned int weapon_id) const;
  bool         FireWeapon(unsigned int weapon_id);
  void         SetWeaponAutofire(unsigned int weapon_id, bool enabled);
  
  unsigned int AddEngine(const glm::vec3 & thrust_direction, double power);
  void         SetEngineThrottle(unsigned int engine_id, double throttle);
  void         UpgradeEngines(double power_multiplier);

  void               UpgradeFromCollectible(ObjectCollectible * collectible);
  SpaceshipUpgrade * GetUpgrade(SpaceshipUpgrade::Type type) const;

  void        SetOwnerGameStats(GameStats * gamestats);
  GameStats * GetOwnerGameStats() const;

  void CopyUpgrades(const ObjectSpaceship & source);

  void         ClearControlPrograms();
  void         AddControlProgram(SpaceshipControlProgram * program);
  void         AddNamedControlProgram(const std::string & name);
  unsigned int GetActiveControlProgramCount() const;
  
private:
  class Engine
  {
  public:
    glm::vec3 _thrust_direction;
    double    _power;
    double    _throttle;
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
  std::vector<Engine *>  _engines;
  std::vector<Weapon *>  _weapons;
  std::vector<SpaceshipUpgrade *> _upgrades;
  std::vector<SpaceshipControlProgram *> _control_programs;
};

#endif
