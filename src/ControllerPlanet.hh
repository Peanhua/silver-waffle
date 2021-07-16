#ifndef CONTROLLER_PLANET_HH_
#define CONTROLLER_PLANET_HH_
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


class ControllerPlanet : public Controller
{
public:
  ControllerPlanet(ObjectSpaceship * spaceship);

  void SteerForward(bool pressed) override;
  void SteerBackward(bool pressed) override;
  void SteerLeft(bool pressed) override;
  void SteerRight(bool pressed) override;

private:
  bool _facing_right;
};

#endif
