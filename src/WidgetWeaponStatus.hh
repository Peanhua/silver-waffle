#ifndef WIDGET_WEAPON_STATUS_HH_
#define WIDGET_WEAPON_STATUS_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "WidgetSpaceshipStatus.hh"


class WidgetWeaponStatus : public WidgetSpaceshipStatus
{
public:
  WidgetWeaponStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship);
  
  void Tick(double deltatime) override;
  void Draw() const override;
  void SetSpaceship(ObjectSpaceship * spaceship) override;

private:
  unsigned int _weaponcount;
  
  void UpdateMeter(bool force = false);
};


#endif

