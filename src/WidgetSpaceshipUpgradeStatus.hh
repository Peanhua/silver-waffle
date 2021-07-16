#ifndef WIDGET_SPACESHIP_UPGRADE_STATUS_HH_
#define WIDGET_SPACESHIP_UPGRADE_STATUS_HH_
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
class SpaceshipUpgrade;


class WidgetSpaceshipUpgradeStatus : public Widget
{
public:
  WidgetSpaceshipUpgradeStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, SpaceshipUpgrade * upgrade);

  void Draw() const override;

  void               SetUpgrade(SpaceshipUpgrade * upgrade);
  SpaceshipUpgrade * GetUpgrade() const;
  
private:
  SpaceshipUpgrade * _upgrade;
  MeshProgressBar *  _meter;
};

#endif
