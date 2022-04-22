/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
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


void ControllerSpace::SteerForward(bool pressed)
{
  _spaceship->SetEngineThrottle(2, pressed ? 1.0 : 0.0);
}


void ControllerSpace::SteerBackward(bool pressed)
{
  _spaceship->SetEngineThrottle(3, pressed ? 1.0 : 0.0);
}
