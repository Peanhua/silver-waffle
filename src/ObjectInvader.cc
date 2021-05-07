#include "ObjectInvader.hh"

ObjectInvader::ObjectInvader(Scene * scene)
  : ObjectSpaceship(scene),
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
      
      if(vel.z > -2.0f)
        {
          imp.z = -0.5f;
          doimp = true;
        }
      if(vel.x < 0.0f)
        {
          imp.x = 0.1f;
          doimp = true;
        }
      else if(vel.x > 0.0f)
        {
          imp.x = -0.1f;
          doimp = true;
        }
      if(doimp)
        AddImpulse(imp);
    }

  ObjectSpaceship::Tick(deltatime);
}

