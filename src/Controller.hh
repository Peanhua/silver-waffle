#ifndef CONTROLLER_HH_
#define CONTROLLER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

class ObjectSpaceship;

class Controller
{
public:
  Controller(ObjectSpaceship * spaceship);
  virtual ~Controller();

  virtual void SteerForward(bool pressed);
  virtual void SteerBackward(bool pressed);
  virtual void SteerLeft(bool pressed);
  virtual void SteerRight(bool pressed);
  virtual void ActivateWeapon(bool pressed);

  ObjectSpaceship * GetOwner() const;
  
protected:
  ObjectSpaceship * _spaceship;
};

#endif
