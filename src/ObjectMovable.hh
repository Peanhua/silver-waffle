#ifndef OBJECT_MOVABLE_HH_
#define OBJECT_MOVABLE_HH_

#include "Object.hh"

class ObjectMovable : public Object
{
public:
  ObjectMovable(Scene * scene);

  void Tick(double deltatime) override;
  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;

  void SetHorizontalPositionLimit(double limit);
  void AddImpulse(const glm::vec3 & impulse);
  void SetMaxVelocity(double max_velocity);
  void SetVelocity(const glm::vec3 & velocity);
  const glm::vec3 & GetVelocity() const;
  void EnableVelocity(bool x = true, bool y = true, bool z = true);

  void SetAngularVelocity(const glm::quat & angular_velocity, double magnitude);

private:
  glm::vec3 _velocity;
  bool      _velocity_enabled[3];
  double    _max_velocity;
  float     _horizontal_position_limit;
  glm::quat _angular_velocity;
  double    _angular_velocity_magnitude;
};

#endif
