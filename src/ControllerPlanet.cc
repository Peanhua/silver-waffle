#include "ControllerPlanet.hh"
#include "ObjectSpaceship.hh"


ControllerPlanet::ControllerPlanet(ObjectSpaceship * spaceship)
  : Controller(spaceship),
    _facing_right(false)
{
}


void ControllerPlanet::SteerForward(bool pressed)
{
  _spaceship->SetEngineThrottle(0 + 4, pressed ? 1.0 : 0.0);
}


void ControllerPlanet::SteerBackward(bool pressed)
{
  _spaceship->SetEngineThrottle(1 + 4, pressed ? 1.0 : 0.0);
}


void ControllerPlanet::SteerLeft(bool pressed)
{
  if(pressed && _facing_right)
    {
      _facing_right = false;
      _spaceship->SetOrientation(glm::quat(1, 0, 0, 0));
      _spaceship->RotateYaw(90);
    }
  _spaceship->SetEngineThrottle(2 + 4, pressed ? 1.0 : 0.0);
  _spaceship->SetEngineThrottle(3 + 4, pressed ? 1.0 : 0.0);
}


void ControllerPlanet::SteerRight(bool pressed)
{
  if(pressed && !_facing_right)
    {
      _facing_right = true;
      _spaceship->SetOrientation(glm::quat(1, 0, 0, 0));
      _spaceship->RotateYaw(-90);
    }
  _spaceship->SetEngineThrottle(2 + 4, pressed ? 1.0 : 0.0);
  _spaceship->SetEngineThrottle(3 + 4, pressed ? 1.0 : 0.0);
}
