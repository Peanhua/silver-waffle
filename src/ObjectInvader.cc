#include "ObjectInvader.hh"

ObjectInvader::ObjectInvader(const glm::vec3 & position)
  : ObjectMovable(position),
    _time_to_think(0.1),
    _next_thinking(0)
{
}

void ObjectInvader::Tick(double deltatime)
{
  _next_thinking -= deltatime;
  if(_next_thinking < 0.0)
    {
      _next_thinking += _time_to_think;
      auto vel = GetVelocity();
      glm::vec3 imp(0, 0, 0);
      bool doimp = false;
      
      if(vel.z > -2)
        {
          imp.z = -0.5;
          doimp = true;
        }
      if(vel.x < 0)
        {
          imp.x = 0.1;
          doimp = true;
        }
      else if(vel.x > 0)
        {
          imp.x = -0.1;
          doimp = true;
        }
      if(doimp)
        AddImpulse(imp);
    }

  ObjectMovable::Tick(deltatime);
}

