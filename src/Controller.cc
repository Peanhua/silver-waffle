#include "Controller.hh"
#include "ObjectSpaceship.hh"
#include <cassert>


Controller::Controller(ObjectSpaceship * spaceship)
  : _spaceship(spaceship)
{
}

Controller::~Controller()
{
}


void Controller::SteerForward(bool pressed)
{
  assert(pressed == pressed);
}


void Controller::SteerBackward(bool pressed)
{
  assert(pressed == pressed);
}


void Controller::SteerLeft(bool pressed)
{
  assert(pressed == pressed);
}


void Controller::SteerRight(bool pressed)
{
  assert(pressed == pressed);
}


void Controller::ActivateWeapon(bool pressed)
{
  for(unsigned int i = 0; i < _spaceship->GetWeaponCount(); i++)
    _spaceship->SetWeaponAutofire(i, pressed);
}


ObjectSpaceship * Controller::GetOwner() const
{
  return _spaceship;
}
