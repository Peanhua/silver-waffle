#ifndef WIDGET_SHOP_ITEM_HH_
#define WIDGET_SHOP_ITEM_HH_
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

class ObjectSpaceship;
class SpaceshipUpgrade;
class UpgradeMaterial;


class WidgetShopItem : public Widget
{
public:
  WidgetShopItem(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                 ObjectSpaceship * spaceship,
                 std::vector<UpgradeMaterial *> & available_materials,
                 SpaceshipUpgrade * upgrade);
  
private:
  ObjectSpaceship *                _spaceship;
  std::vector<UpgradeMaterial *> & _available_materials;
  SpaceshipUpgrade *               _upgrade;

  bool CanBuy() const;
};

#endif
