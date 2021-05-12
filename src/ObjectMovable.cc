#include "ObjectMovable.hh"
#include <iostream>

ObjectMovable::ObjectMovable(Scene * scene)
  : Object(scene),
    _velocity(glm::vec3(0, 0, 0)),
    _max_velocity(10),
    _angular_velocity(0, 0, 0, 0)
{
}


void ObjectMovable::Tick(double deltatime)
{
  Translate(_velocity * static_cast<float>(deltatime));

  if(glm::length(_angular_velocity) > 0.01f)
    {
      glm::quat src(1, 0, 0, 0);
      glm::quat dst = _angular_velocity;
      glm::quat res = glm::slerp(src, dst, static_cast<float>(deltatime));
      Rotate(res);
    }
  
  Object::Tick(deltatime);
}


void ObjectMovable::Hit(double damage, const glm::vec3 & impulse)
{
  Object::Hit(damage, impulse);
  AddImpulse(impulse);
}


void ObjectMovable::AddImpulse(const glm::vec3 & impulse)
{
  _velocity += impulse;
  double speed = glm::length(_velocity);
  if(speed > _max_velocity)
    _velocity = _velocity / static_cast<float>((1.0 + (speed - _max_velocity) / _max_velocity));
}


void ObjectMovable::SetMaxVelocity(double max_velocity)
{
  _max_velocity = max_velocity;
}


void ObjectMovable::SetVelocity(const glm::vec3 & velocity)
{
  _velocity = velocity;
}


const glm::vec3 & ObjectMovable::GetVelocity() const
{
  return _velocity;
}


void ObjectMovable::SetAngularVelocity(const glm::quat & angular_velocity)
{
  _angular_velocity = angular_velocity;
}


void ObjectMovable::AddAngularImpulse(const glm::quat & impulse)
{
  _angular_velocity *= impulse;
}
