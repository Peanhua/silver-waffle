/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "../config.h"
#include "WidgetSpaceshipMaintenance.hh"
#include "Font.hh"
#include "GameStats.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "UpgradeMaterial.hh"
#include "WidgetButton.hh"
#include "WidgetShopItem.hh"
#include "WidgetUpgradeMaterial.hh"
#if HAVE_FMT
# include <fmt/core.h>
using fmt::format;
#else
# include <format>
using std::format;
#endif
#include <iostream>


WidgetSpaceshipMaintenance::WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                                                       ObjectSpaceship * spaceship, GameStats * gamestats)

  : Widget(parent, position, size),
    _spaceship(spaceship)
{
  auto font = AssetLoader->LoadFont(20);
  auto font_weight = 0.45f;
  auto font_color = glm::vec3(0.2, 1, 0.2);
  
  SetImage("PanelBorders");
  SetImageColor(glm::vec4(font_color * 0.5f, 1.0));
  SetIsFocusable(false);
        
  int x = 40;
  int y = 20;
  {
    const std::string t("SPACESHIP MAINTENANCE");
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2((GetSize().x - static_cast<int>(tlen)) / 2, y), glm::ivec2(tlen, 30));
    w->SetTextColor(font_color);
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    y += w->GetSize().y + 20;
  }
  font = AssetLoader->LoadFont(16);
  font_weight = 0.25f;
  font_color = glm::vec3(0, 1, 0);

  auto fmt = [](double v)
  {
    auto rv = std::to_string(v);
    return rv.substr(0, rv.find('.') + 2);
  };
  {
    const std::string t("Hull health: " + fmt(_spaceship->GetHealth()));
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(tlen, 30));
    w->SetTextFont(font);
    w->SetTextColor(font_color);
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    y += w->GetSize().y;
  }
  {
    auto u = _spaceship->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
    if(u->GetInstallCount() > 0)
      {
        const std::string t("Warp fuel: " + fmt(u->GetTimer()) + " / " + fmt(u->GetTimerMax()));
        const double tlen = font->GetWidth(t);
        auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(tlen, 30));
        w->SetTextFont(font);
        w->SetTextColor(font_color);
        w->SetText(t);
        w->SetTextFontWeight(font_weight);
        w->SetIsFocusable(false);
        y += w->GetSize().y;
      }
  }

  y += 20;
  {
    auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(350, 30));
    w->SetTextFont(font);
    w->SetTextColor(font_color);
    w->SetText(format("Blueprint points: {}", gamestats->GetBlueprintPoints()));
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    y += w->GetSize().y;
  }
  {
    {
      auto label = new Widget(this, glm::ivec2(x, y), glm::ivec2(350, 30));
      std::string labeltext("Upgrade modules:");
      label->SetTextFont(font);
      label->SetTextColor(font_color);
      label->SetText(labeltext);
      label->SetTextFontWeight(font_weight);
      label->SetIsFocusable(false);
      y += label->GetSize().y;
    }
    std::vector<SpaceshipUpgrade::Type> types
      {
        SpaceshipUpgrade::Type::WEAPON,
        SpaceshipUpgrade::Type::WEAPON_COOLER,
        SpaceshipUpgrade::Type::ENGINE_UPGRADE,
        SpaceshipUpgrade::Type::HULL_UPGRADE,
        SpaceshipUpgrade::Type::REPAIR_DROID,
        SpaceshipUpgrade::Type::WARP_ENGINE,
        SpaceshipUpgrade::Type::PLANET_LANDER,
      };
    for(auto t : types)
      if(gamestats->IsSpaceshipUpgradeAvailable(t))
        {
          auto w = new WidgetShopItem(this, glm::ivec2(x + 20, y), glm::ivec2(200, 30),
                                      _spaceship, gamestats->GetUpgradeMaterials(), _spaceship->GetUpgrade(t));
          y += w->GetSize().y;
        }
      else
        {
          auto w = new WidgetButton(this, {x + 20, y}, {500, 30});
          w->SetTextFont(font);
          w->SetTextColor(font_color);
          w->SetText(format("Unlock {}", _spaceship->GetUpgrade(t)->GetName()));
          w->SetTextFontWeight(font_weight);
          w->SetTextPaddingCentered(true, true);
          y += w->GetSize().y;
          w->SetOnClicked([w, gamestats, t](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & pos)
          {
            if(!pressed)
              return;
            if(gamestats->GetBlueprintPoints() > 0)
              {
                gamestats->UnlockSpaceshipUpgrade(t);
                w->Destroy();
              }
          });
        }
  }
  
  y += 20;
  {
    auto label = new Widget(this, glm::ivec2(x, y), glm::ivec2(350, 30));
    std::string labeltext("Materials:");
    label->SetTextFont(font);
    label->SetTextColor(font_color);
    label->SetText(labeltext);
    label->SetTextFontWeight(font_weight);
    label->SetIsFocusable(false);

    int xx = x + 20 + 300;
    for(auto m : gamestats->GetUpgradeMaterials())
      {
        auto w = new WidgetUpgradeMaterial(this, glm::ivec2(xx, y + 2), glm::ivec2(50, 25), *m);
        w->SetIsFocusable(false);
        xx += 50 + 2;
      }
    
    y += label->GetSize().y;
  }
}
