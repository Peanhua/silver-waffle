/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetShopItem.hh"
#include "Font.hh"
#include "ObjectSpaceship.hh"
#include "SpaceshipUpgrade.hh"
#include "SubsystemAssetLoader.hh"
#include "WidgetButton.hh"
#include "WidgetUpgradeMaterial.hh"


WidgetShopItem::WidgetShopItem(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                               ObjectSpaceship * spaceship,
                               std::vector<UpgradeMaterial *> & available_materials,
                               SpaceshipUpgrade * upgrade)
  : Widget(parent, position, size),
    _spaceship(spaceship),
    _available_materials(available_materials),
    _upgrade(upgrade)
{
  auto font = AssetLoader->LoadFont(16);
  auto font_weight = 0.25f;
  auto font_color = glm::vec3(0, 1, 0);

  int x = 0;
  int y = 0;

  {
    const std::string t(_upgrade->GetName() + ": " + std::to_string(_upgrade->GetInstallCount()) + "/" + std::to_string(_upgrade->GetMaxInstalls()));
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(tlen, 30));
    w->SetTextColor(font_color);
    w->SetTextFont(font);
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    x += 300;

    std::vector<UpgradeMaterial::Type> types
      {
        UpgradeMaterial::Type::ATTACK,
        UpgradeMaterial::Type::DEFENSE,
        UpgradeMaterial::Type::PHYSICAL
      };
    for(auto type : types)
      {
        auto wum = new WidgetUpgradeMaterial(this, glm::ivec2(x, y), glm::ivec2(50, 25), type);
        wum->SetUpgradeMaterialAmount(_upgrade->GetNextPurchaseCost(type));
        x += wum->GetSize().x + 2;
      }
    
    if(CanBuy())
      {
        const std::string bt("Buy");
        const double buytlen = font->GetWidth(bt);
        auto buybutton = new WidgetButton(this, glm::ivec2(x, y), glm::ivec2(buytlen, 25));
        buybutton->SetTextColor(font_color);
        buybutton->SetTextFont(font);
        buybutton->SetText(bt);
        buybutton->SetTextFontWeight(font_weight);
        buybutton->SetOnClicked([this, w, buybutton](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & pos)
        {
          if(!pressed && CanBuy())
            {
              for(auto m : _available_materials)
                m->Use(_upgrade->GetNextPurchaseCost(m->GetType()));
              _upgrade->Install();
              const std::string t2(_upgrade->GetName() + ": " + std::to_string(_upgrade->GetInstallCount()) + "/" + std::to_string(_upgrade->GetMaxInstalls()));
              w->SetText(t2);
            }
          if(!CanBuy())
            {
              //buybutton->Destroy(); Should really do this, but not doing because of buggy Screen->_focused_widget.
              buybutton->SetText("");
              buybutton->SetIsFocused(false);
              buybutton->SetIsFocusable(false);
            }
        });
      }

  }
}


bool WidgetShopItem::CanBuy() const
{
  if(!_upgrade->CanBeInstalled())
    return false;
  for(auto m : _available_materials)
    if(m->GetAmount() < _upgrade->GetNextPurchaseCost(m->GetType()))
      return false;
  return true;
}

