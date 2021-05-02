#include "ObjectInvader.hh"

ObjectInvader::ObjectInvader(const glm::vec3 & position)
  : ObjectMovable(position),
    _time_to_think(0),
    _next_thinking(0)
{
  SetVelocity(glm::vec3(0.0, 0.0, -0.5));
}

void ObjectInvader::Tick(double deltatime)
{
  _next_thinking -= deltatime;
  if(_next_thinking < 0.0)
    {
      _next_thinking += _time_to_think;
      
    }

  ObjectMovable::Tick(deltatime);
}

