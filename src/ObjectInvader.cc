#include "ObjectInvader.hh"


ObjectInvader::ObjectInvader(Scene * scene, unsigned int random_seed)
  : ObjectSpaceship(scene),
    _random_generator(random_seed),
    _time_to_think(0.334),
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

      double r = static_cast<double>(_random_generator() - _random_generator.min()) / static_cast<double>(_random_generator.max());
      if(r < 0.01 * _time_to_think)
        FireWeapon(0);
    }

  ObjectSpaceship::Tick(deltatime);
}

