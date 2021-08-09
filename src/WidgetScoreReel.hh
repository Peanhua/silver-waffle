#ifndef WIDGET_SCORE_REEL_HH_
#define WIDGET_SCORE_REEL_HH_
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

class ScoreReel;
class TextureRenderer;


class WidgetScoreReel : public Widget
{
public:
  WidgetScoreReel(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ScoreReel * score_reel);
  ~WidgetScoreReel();
  
  void Tick(double deltatime) override;
  void Draw()           const override;
  
private:
  ScoreReel *       _score_reel;
  TextureRenderer * _texture_renderer;
  bool              _should_redraw;
};

#endif
