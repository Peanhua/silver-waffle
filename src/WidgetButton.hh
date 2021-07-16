#ifndef WIDGET_BUTTON_HH_
#define WIDGET_BUTTON_HH_
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

class Mesh;


class WidgetButton : public Widget
{
public:
  WidgetButton(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);
  ~WidgetButton();

  void Draw() const override;
  
private:
  Mesh * _mesh_up;
  Mesh * _mesh_down;
};

#endif
