#ifndef OBJECT_BULLET_HH_
#define OBJECT_BULLET_HH_

#include "ObjectMovable.hh"

class ObjectBullet : public ObjectMovable
{
public:
  ObjectBullet();

  void Tick(double deltatime) override;

  void Activate(const glm::vec3 & position, const glm::vec3 & velocity, double lifetime);

private:
  double _lifetime_left;
};

#endif
