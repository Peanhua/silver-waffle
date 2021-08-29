/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
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


void Controller::SteerForward([[maybe_unused]] bool pressed)
{
}


void Controller::SteerBackward([[maybe_unused]] bool pressed)
{
}


void Controller::SteerLeft([[maybe_unused]] bool pressed)
{
}


void Controller::SteerRight([[maybe_unused]] bool pressed)
{
}


void Controller::ActivateWeaponGroup(unsigned int weapon_group, bool pressed)
{
  _spaceship->SetWeaponGroupAutofire(weapon_group, pressed);
}


ObjectSpaceship * Controller::GetOwner() const
{
  return _spaceship;
}
