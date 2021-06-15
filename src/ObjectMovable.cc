#include "ObjectMovable.hh"
#include <iostream>

ObjectMovable::ObjectMovable(Scene * scene)
  : Object(scene),
    _velocity(glm::vec3(0, 0, 0)),
    _max_velocity(10),
    _horizontal_position_limit(0),
    _angular_velocity(0, 0, 0, 0),
    _angular_velocity_magnitude(0)
{
  _velocity_enabled[0] =
    _velocity_enabled[1] =
    _velocity_enabled[2] = true;
}


void ObjectMovable::Tick(double deltatime)
{
  Translate(_velocity * static_cast<float>(deltatime));

  if(_angular_velocity_magnitude > 0.01)
    {
      glm::quat src(1, 0, 0, 0);
      glm::quat dst = _angular_velocity;
      glm::quat res = glm::slerp(src, dst, static_cast<float>(_angular_velocity_magnitude * deltatime));
      Rotate(res);
    }
  
  if(_horizontal_position_limit > 0.0f)
    {
      const auto pos = GetPosition();
      const auto vel = GetVelocity();
      if(pos.x < -_horizontal_position_limit)
        {
          SetPosition(glm::vec3(-_horizontal_position_limit, pos.yz()));
          if(vel.x < 0.0f)
            SetVelocity(glm::vec3(0.0f, vel.yz()));
        }
      else if(pos.x > _horizontal_position_limit)
        {
          SetPosition(glm::vec3(_horizontal_position_limit, pos.yz()));
          if(vel.x > 0.0f)
            SetVelocity(glm::vec3(0.0f, vel.yz()));
        }
    }

  Object::Tick(deltatime);
}


void ObjectMovable::SetHorizontalPositionLimit(double limit)
{
  _horizontal_position_limit = static_cast<float>(limit);
}
  


void ObjectMovable::Hit(Object * perpetrator, double damage, const glm::vec3 & impulse, bool use_fx)
{
  Object::Hit(perpetrator, damage, impulse, use_fx);
  AddImpulse(impulse);
}


void ObjectMovable::AddImpulse(const glm::vec3 & impulse)
{
  auto v = _velocity + impulse;
  double speed = glm::length(v);
  if(speed > _max_velocity)
    v = v / static_cast<float>((1.0 + (speed - _max_velocity) / _max_velocity));
  SetVelocity(v);
}


void ObjectMovable::SetMaxVelocity(double max_velocity)
{
  _max_velocity = max_velocity;
}


void ObjectMovable::SetVelocity(const glm::vec3 & velocity)
{
  _velocity = velocity;
  if(!_velocity_enabled[0]) _velocity.x = 0;
  if(!_velocity_enabled[1]) _velocity.y = 0;
  if(!_velocity_enabled[2]) _velocity.z = 0;
}


const glm::vec3 & ObjectMovable::GetVelocity() const
{
  return _velocity;
}


void ObjectMovable::SetAngularVelocity(const glm::quat & angular_velocity, double magnitude)
{
  _angular_velocity = angular_velocity;
  _angular_velocity_magnitude = magnitude;
}


void ObjectMovable::EnableVelocity(bool x, bool y, bool z)
{
  _velocity_enabled[0] = x;
  _velocity_enabled[1] = y;
  _velocity_enabled[2] = z;
}

