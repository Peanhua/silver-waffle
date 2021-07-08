#include "ControllerSpace.hh"
#include "ObjectSpaceship.hh"


void ControllerSpace::SteerLeft(bool pressed)
{
  _spaceship->SetEngineThrottle(0, pressed ? 1.0 : 0.0);
}


void ControllerSpace::SteerRight(bool pressed)
{
  _spaceship->SetEngineThrottle(1, pressed ? 1.0 : 0.0);
}
