#include "WidgetSpaceshipMaintenance.hh"
#include "Font.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "UpgradeMaterial.hh"
#include "WidgetButton.hh"
#include "WidgetShopItem.hh"
#include <iostream>


WidgetSpaceshipMaintenance::WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                                                       ObjectSpaceship * spaceship, std::vector<UpgradeMaterial *> & materials)

  : Widget(parent, position, size),
    _spaceship(spaceship)
{
  auto font = AssetLoader->LoadFont(20);
  auto font_weight = 0.5f;
  auto font_color = glm::vec3(0, 1, 1);
  
  SetImage("PanelBorders");
  SetIsFocusable(false);
        
  int x = 10;
  int y = 2;
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
  {
    const std::string t("Hull health: " + std::to_string(static_cast<int>(_spaceship->GetHealth())));
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(tlen, 30));
    w->SetTextFont(font);
    w->SetTextColor(font_color);
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    y += w->GetSize().y + 4;
  }

  std::vector<SpaceshipUpgrade::Type> types
    {
      SpaceshipUpgrade::Type::WEAPON,
      SpaceshipUpgrade::Type::WEAPON_COOLER
    };
  for(auto t : types)
    {
      auto w = new WidgetShopItem(this, glm::ivec2(x, y), glm::ivec2(200, 30),
                                  _spaceship, materials, _spaceship->GetUpgrade(t));
      y += w->GetSize().y + 4;
    }
}

