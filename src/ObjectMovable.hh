#ifndef OBJECT_MOVABLE_HH_
#define OBJECT_MOVABLE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Object.hh"

class ObjectMovable : public Object
{
public:
  ObjectMovable(Scene * scene, unsigned int random_seed);

  void Tick(double deltatime) override;
  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;
  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;

  void AddImpulse(const glm::vec3 & impulse);
  void SetVelocity(const glm::vec3 & velocity);
  const glm::vec3 & GetVelocity() const;
  void EnableVelocity(bool x = true, bool y = true, bool z = true);
  void EnableHitImpulse(bool enabled);
  void SetBounciness(double bounciness);
  
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
  double    _bounciness;
};

#endif
