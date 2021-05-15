#ifndef OBJECT_SPACESHIP_HH_
#define OBJECT_SPACESHIP_HH_

#include "ObjectMovable.hh"
#include <vector>


class ObjectSpaceship : public ObjectMovable
{
public:
  using ObjectMovable::ObjectMovable;

  void Tick(double deltatime) override;

  unsigned int AddEngine(const glm::vec3 & thrust_direction, double power);
  unsigned int AddWeapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage);
  
  void FireWeapon(unsigned int weapon_id);
  void SetEnginePower(unsigned int engine_id, double throttle);

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
    Mesh *    _projectile;
    glm::vec3 _projectile_direction;
    double    _projectile_initial_velocity;
    double    _projectile_damage;
  private:
  };
  
  std::vector<Engine *> _engines;
  std::vector<Weapon *> _weapons;
};

#endif
