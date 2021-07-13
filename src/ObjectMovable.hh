#ifndef OBJECT_MOVABLE_HH_
#define OBJECT_MOVABLE_HH_

#include "Object.hh"

class ObjectMovable : public Object
{
public:
  ObjectMovable(Scene * scene, unsigned int random_seed);

  void Tick(double deltatime) override;
  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;

  void AddImpulse(const glm::vec3 & impulse);
  void SetVelocity(const glm::vec3 & velocity);
  const glm::vec3 & GetVelocity() const;
  void EnableVelocity(bool x = true, bool y = true, bool z = true);
  void EnableHitImpulse(bool enabled);
  
  void SetAngularVelocity(const glm::quat & angular_velocity, double magnitude);

  void SetIsAffectedByGravity(bool is_affected);

private:
  glm::vec3 _velocity;
  bool      _velocity_enabled[3];
  double    _max_velocity;
  glm::quat _angular_velocity;
  double    _angular_velocity_magnitude;
  bool      _hit_impulse_enabled;
  bool      _is_affected_by_gravity;
};

#endif
