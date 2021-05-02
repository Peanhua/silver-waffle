#include "ObjectMovable.hh"
#include <iostream>

ObjectMovable::ObjectMovable(const glm::vec3 & position)
  : Object(position),
    _velocity(glm::vec3(0, 0, 0)),
    _max_velocity(10)
{
}


void ObjectMovable::Tick(double deltatime)
{
  Object::Tick(deltatime);
  SetPosition(GetPosition() + _velocity * static_cast<float>(deltatime));
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
