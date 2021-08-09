/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetScoreReel.hh"
#include "Image.hh"
#include "ScoreReel.hh"
#include "TextureRenderer.hh"


WidgetScoreReel::WidgetScoreReel(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ScoreReel * score_reel)
  : Widget(parent, position, size),
    _score_reel(score_reel),
    _should_redraw(true)
{
  assert(_score_reel);
  _texture_renderer = new TextureRenderer(512, 128);
  SetImage(new Image(true));
}


WidgetScoreReel::~WidgetScoreReel()
{
  delete _texture_renderer;
}


void WidgetScoreReel::Tick(double deltatime)
{
  Widget::Tick(deltatime);
  
  _should_redraw = _score_reel->Tick(deltatime);
}


void WidgetScoreReel::Draw() const
{
  if(_should_redraw)
    {
      glEnable(GL_DEPTH_TEST);
      _texture_renderer->BeginRender();
      _score_reel->Draw();
      _texture_renderer->EndRender();
      GetImage()->SetTextureId(_texture_renderer->GetTextureId());
    }
  Widget::Draw();
}
