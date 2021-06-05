#ifndef OBJECT_SPACESHIP_HH_
#define OBJECT_SPACESHIP_HH_

#include "ObjectMovable.hh"
#include <vector>

class ObjectCollectible;


class ObjectSpaceship : public ObjectMovable
{
public:
  class Upgrade
  {
  public:
    enum class Type
      {
        BONUS_DAMAGE,
        SHIELD
      };

    Upgrade(Type type);
    
    void Add(double amount, double time);
    void Tick(double deltatime);
    void AddFromCollectible(ObjectCollectible * collectible);

    bool   IsActive() const;
    Type   GetType()  const;
    double GetValue() const;
    double GetTimer() const;
    
  private:
    Type   _type;
    double _value;
    double _timer;
  };


  ObjectSpaceship(Scene * scene);

  void Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const override;
  void Tick(double deltatime) override;
  void Hit(double damage, const glm::vec3 & impulse) override;

  unsigned int AddWeapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage);
  unsigned int GetWeaponCount() const;
  double       GetWeaponHeat(unsigned int weapon_id) const;
  bool         FireWeapon(unsigned int weapon_id);
  void         SetWeaponAutofire(unsigned int weapon_id, bool enabled);
  
  unsigned int AddEngine(const glm::vec3 & thrust_direction, double power);
  void         SetEnginePower(unsigned int engine_id, double throttle);

  void      AddUpgrade(Upgrade::Type type, double value, double time);
  void      UpgradeFromCollectible(ObjectCollectible * collectible);
  Upgrade * GetUpgrade(Upgrade::Type type) const;
  
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
    glm::vec3 _location;
    bool      _autofire;
    double    _heat;
    Mesh *    _projectile;
    glm::vec3 _projectile_direction;
    double    _projectile_initial_velocity;
    double    _projectile_damage;
  private:
  };

  
  std::vector<Engine *>  _engines;
  std::vector<Weapon *>  _weapons;
  std::vector<Upgrade *> _upgrades;
};

#endif
