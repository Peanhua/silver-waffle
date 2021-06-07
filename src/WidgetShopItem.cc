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
    const std::string t(_upgrade->GetName() + ": " + std::to_string(_upgrade->GetIntValue()) + "/" + std::to_string(_upgrade->GetMaxIntValue()));
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
        buybutton->SetOnClicked([this, w, buybutton](bool pressed, unsigned int button, const glm::ivec2 & pos)
        {
          assert(button == button);
          assert(pos == pos);

          if(!pressed && CanBuy())
            {
              for(auto m : _available_materials)
                m->Use(_upgrade->GetNextPurchaseCost(m->GetType()));
              _upgrade->Add(1, 0);
              const std::string t2(_upgrade->GetName() + ": " + std::to_string(_upgrade->GetIntValue()) + "/" + std::to_string(_upgrade->GetMaxIntValue()));
              w->SetText(t2);
            }
          if(!CanBuy())
            buybutton->Destroy();
        });
      }

  }
}


bool WidgetShopItem::CanBuy() const
{
  if(!_spaceship->CanAddUpgrade(_upgrade->GetType()))
    return false;
  for(auto m : _available_materials)
    if(m->GetAmount() < _upgrade->GetNextPurchaseCost(m->GetType()))
      return false;
  return true;
}

