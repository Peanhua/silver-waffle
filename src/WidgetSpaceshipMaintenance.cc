#include "WidgetSpaceshipMaintenance.hh"
#include "Font.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "WidgetButton.hh"
#include <iostream>


WidgetSpaceshipMaintenance::WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship, get_material_amount_t GetMaterialAmount, use_material_t UseMaterial)
  : Widget(parent, position, size),
    _spaceship(spaceship)
{
  auto font = AssetLoader->LoadFont(20);
  auto font_weight = 0.5f;
  
  SetImage("PanelBorders");
  SetIsFocusable(false);
        
  int x = 10;
  int y = 2;
  {
    const std::string t("SPACESHIP MAINTENANCE");
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2((GetSize().x - static_cast<int>(tlen)) / 2, y), glm::ivec2(tlen, 30));
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    y += w->GetSize().y + 20;
  }
  font = AssetLoader->LoadFont(16);
  font_weight = 0.25f;
  {
    const std::string t("Hull health: " + std::to_string(static_cast<int>(_spaceship->GetHealth())));
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(tlen, 30));
    w->SetTextFont(font);
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    y += w->GetSize().y + 4;
  }
  {
    const std::string t("Weapon count: " + std::to_string(_spaceship->GetWeaponCount()));
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2(x, y), glm::ivec2(tlen, 30));
    w->SetTextFont(font);
    w->SetText(t);
    w->SetTextFontWeight(font_weight);
    w->SetIsFocusable(false);
    
    unsigned int costs[] { 10, 0, 10 };

    auto CanBuy = [this, GetMaterialAmount, costs]() -> bool
    {
      if(_spaceship->GetWeaponCount() >= 5)
        return false;
      for(int i = 0; i < 3; i++)
        if(GetMaterialAmount(i) < costs[i])
          return false;
      return true;
    };

    if(CanBuy())
      {
        const std::string bt("Buy");
        const double buytlen = font->GetWidth(bt);
        auto buybutton = new WidgetButton(this, glm::ivec2(x + w->GetSize().x + 20, y), glm::ivec2(buytlen, 30));
        buybutton->SetTextFont(font);
        buybutton->SetText(bt);
        buybutton->SetTextFontWeight(font_weight);
        buybutton->SetOnClicked([this, w, costs, buybutton, CanBuy, UseMaterial](bool pressed, unsigned int button, const glm::ivec2 & pos)
        {
          assert(button == button);
          assert(pos == pos);

          if(!pressed && CanBuy())
            {
              _spaceship->AddWeapon();
              for(unsigned int i = 0; i < 3; i++)
                UseMaterial(i, costs[i]);
              w->SetText(std::string("Weapon count: ") + std::to_string(_spaceship->GetWeaponCount()));
            }
          if(!CanBuy())
            buybutton->Destroy();
        });
      }

    y += w->GetSize().y + 4;
  }
        
}

