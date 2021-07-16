#ifndef WIDGET_PLAYER_SHIP_HH_
#define WIDGET_PLAYER_SHIP_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "glm.hh"
#include "Widget.hh"

class Mesh;
class TextureRenderer;


class WidgetPlayerShip : public Widget
{
public:
  WidgetPlayerShip(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);
  ~WidgetPlayerShip();
  
  void Tick(double deltatime) override;


private:
  Mesh *            _mesh;
  TextureRenderer * _texture_renderer;
};

#endif
