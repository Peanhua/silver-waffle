#ifndef OBJECT_MOVABLE_HH_
#define OBJECT_MOVABLE_HH_

#include "Object.hh"

class ObjectMovable : public Object
{
public:
  ObjectMovable(const glm::vec3 & position);

  void Tick(double deltatime) override;

  void AddImpulse(const glm::vec3 & impulse);
  void SetMaxVelocity(double max_velocity);
  void SetVelocity(const glm::vec3 & velocity);
  const glm::vec3 & GetVelocity() const;

private:
  glm::vec3 _velocity;
  double    _max_velocity;
};

#endif
