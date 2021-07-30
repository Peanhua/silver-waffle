#ifndef WIDGET_HIGHSCORES_HH_
#define WIDGET_HIGHSCORES_HH_
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

class HighscoreEntry;


class WidgetHighscores : public Widget
{
public:
  WidgetHighscores(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, bool is_dialog);

  void Tick(double deltatime) override;

  void Refresh();
  
private:
  bool             _is_dialog;
  unsigned int     _last_refresh;
};

#endif
