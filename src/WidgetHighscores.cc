/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetHighscores.hh"
#include "Font.hh"
#include "HighscoreEntry.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemHighscores.hh"
#include "WidgetButton.hh"


WidgetHighscores::WidgetHighscores(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, bool is_dialog)
  : Widget(parent, position, size),
    _is_dialog(is_dialog),
    _last_refresh(0)
{
  SetIsFocusable(false);
  Refresh();
}


void WidgetHighscores::Tick(double deltatime)
{
  Widget::Tick(deltatime);
  
  auto cursize = static_cast<unsigned int>(Highscores->Get().size());
  if(cursize != _last_refresh)
    {
      _last_refresh = cursize;
      Refresh();
    }
}


void WidgetHighscores::Refresh()
{
  DestroyChildren();

  auto font_weight = 0.3f;
  auto font_color = glm::vec3(0, 1, 1);

  const int panelwidth = 300;
  const int panelheight = std::min(GetSize().y, 10 + 30 + 5 * 25 + 10 + (_is_dialog ? 30 + 10 : 0));
  auto panel = new Widget(this,
                          (GetSize() - glm::ivec2(panelwidth, panelheight)) / 2,
                          glm::ivec2(panelwidth, panelheight));
  if(_is_dialog)
    {
      panel->SetImage("PanelBorders");
      panel->SetImageColor(glm::vec4(font_color * 0.5f, 1.0f));
    }
  panel->SetIsFocusable(false);

  int y = 10;

  auto title = new Widget(panel, glm::ivec2(0, y), glm::ivec2(panelwidth, 30));
  title->SetText("Highscores:");
  title->SetTextFont(AssetLoader->LoadFont(18));
  title->SetTextFontWeight(font_weight * 1.2f);
  title->SetTextPaddingCentered(true, true);
  title->SetTextColor(glm::vec3(0, 1, 1));
  y += title->GetSize().y;

  auto font = AssetLoader->LoadFont(12);
  auto fonthi = AssetLoader->LoadFont(13);
  auto entries = Highscores->Get();
  for(unsigned int i = 0; i < 5 && i < entries.size(); i++)
    {
      auto entry = entries[i];
      int rowheight;
      auto x = title->GetPosition().x + 10;
      {
        auto w = new Widget(panel, glm::ivec2(x, y), glm::ivec2(150, 25));
        w->SetText(std::to_string(i + 1) + ".  " + std::to_string(entry->GetScore()));
        if(entry == Highscores->GetLast())
          {
            w->SetTextFont(fonthi);
            w->SetTextColor(font_color);
            w->SetTextFontWeight(font_weight * 1.1f);
          }
        else
          {
            w->SetTextFont(font);
            w->SetTextColor(font_color * 0.5f);
            w->SetTextFontWeight(font_weight);
          }
        w->SetTextPaddingCentered(false, true);
        x += w->GetSize().x;
        rowheight = w->GetSize().y;
      }
      {
        auto w = new Widget(panel, glm::ivec2(x, y + 3), glm::ivec2(20, 20));
        if(entry->GameCompleted())
          w->SetImage("Checkmark-Green");
        x += w->GetSize().x + 10;
        rowheight = std::max(rowheight, w->GetSize().y);
      }
      {
        auto w = new Widget(panel, glm::ivec2(x, y), glm::ivec2(100, 25));
        w->SetText(std::to_string(static_cast<unsigned int>(entry->GetGameTime())) + "s");
        w->SetTextFont(font);
        w->SetTextColor(font_color * 0.5f);
        w->SetTextFontWeight(font_weight);
        w->SetTextPaddingCentered(false, true);
        x += w->GetSize().x;
        rowheight = std::max(rowheight, w->GetSize().y);
      }
      y += rowheight;
    }

  if(_is_dialog)
    {
      y += 10;
      std::string t = "Close";
      auto ww = 10 + font->GetWidth(t) + 10;
      auto w = new WidgetButton(panel, glm::ivec2((panelwidth - ww) / 2, y), glm::ivec2(ww, 30));
      y += w->GetSize().y;
      w->SetText(t);
      w->SetTextFont(font);
      w->SetTextFontWeight(font_weight * 1.1f);
      w->SetTextColor(font_color);
      w->SetTextPaddingCentered(true, true);
      w->SetOnClicked([this](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & pos)
      {
        if(pressed)
          Destroy();
      });
    }
}
