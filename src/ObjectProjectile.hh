#ifndef OBJECT_PROJECTILE_HH_
#define OBJECT_PROJECTILE_HH_

#include "ObjectMovable.hh"

class ObjectProjectile : public ObjectMovable
{
public:
  ObjectProjectile(Scene * scene);

  void Tick(double deltatime) override;

  void Activate(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);

  Object * GetOwner() const;
  double   GetDamage() const;
  
private:
  Object * _owner;
  double   _damage;
  double   _lifetime_left;
};

#endif
