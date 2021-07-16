#ifndef WIDGET_SPACESHIP_STATUS_HH_
#define WIDGET_SPACESHIP_STATUS_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Widget.hh"

class MeshProgressBar;
class ObjectSpaceship;


class WidgetSpaceshipStatus : public Widget
{
public:
  WidgetSpaceshipStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship);

  void Draw() const override;

  virtual void      SetSpaceship(ObjectSpaceship * spaceship);
  ObjectSpaceship * GetSpaceship() const;

protected:
  ObjectSpaceship * _spaceship;
  MeshProgressBar * _meter;
};


#endif

