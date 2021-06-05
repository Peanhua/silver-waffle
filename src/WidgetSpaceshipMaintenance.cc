#include "WidgetSpaceshipMaintenance.hh"
#include "Font.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "WidgetButton.hh"


WidgetSpaceshipMaintenance::WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship, get_material_amount_t GetMaterialAmount, use_material_t UseMaterial)
  : Widget(parent, position, size),
    _spaceship(spaceship)
{
  auto font = AssetLoader->LoadFont(20);
  auto font_weight = 0.5f;
  
  const glm::ivec2 panelsize(600, 500);
  
  SetImage("PanelBorders");
  SetIsFocusable(false);
        
  int x = 10;
  int y = 2;
  {
    const std::string t("SPACESHIP MAINTENANCE");
    const double tlen = font->GetWidth(t);
    auto w = new Widget(this, glm::ivec2((panelsize.x - static_cast<int>(tlen)) / 2, y), glm::ivec2(tlen, 30));
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
    
    unsigned int cost = 10;
    if(GetMaterialAmount(0) >= cost)
      {
        const std::string bt("Buy");
        const double buytlen = font->GetWidth(bt);
        auto buybutton = new WidgetButton(this, glm::ivec2(x + w->GetSize().x + 20, y), glm::ivec2(buytlen, 30));
        buybutton->SetTextFont(font);
        buybutton->SetText(bt);
        buybutton->SetTextFontWeight(font_weight);
        buybutton->SetOnClicked([this, w, cost, GetMaterialAmount, UseMaterial](bool pressed, unsigned int button, const glm::ivec2 & pos)
        {
          assert(button == button);
          assert(pos == pos);

          if(!pressed)
            {
              if(GetMaterialAmount(0) >= cost)
                {
                  unsigned int materialuse = cost;
                  switch(_spaceship->GetWeaponCount())
                    {
                    case 1:
                      _spaceship->AddWeapon(glm::vec3(-0.1, 1, 0),
                                            AssetLoader->LoadMesh("Projectile"),
                                            glm::normalize(glm::vec3(-0.1, 1, 0)),
                                            10.0,
                                            34.0);
                      break;
                    case 2:
                      _spaceship->AddWeapon(glm::vec3(0.1, 1, 0),
                                            AssetLoader->LoadMesh("Projectile"),
                                            glm::normalize(glm::vec3(0.1, 1, 0)),
                                            10.0,
                                            34.0);
                      break;
                    case 3:
                      _spaceship->AddWeapon(glm::vec3(-0.2, 1, 0),
                                            AssetLoader->LoadMesh("Projectile"),
                                            glm::normalize(glm::vec3(-0.2, 1, 0)),
                                            10.0,
                                            34.0);
                      break;
                    case 4:
                      _spaceship->AddWeapon(glm::vec3(0.2, 1, 0),
                                            AssetLoader->LoadMesh("Projectile"),
                                            glm::normalize(glm::vec3(0.2, 1, 0)),
                                            10.0,
                                            34.0);
                      break;
                    default:
                      materialuse = 0;
                      break;
                    }
                  if(materialuse > 0)
                    UseMaterial(0, materialuse);
                  w->SetText(std::string("Weapon count: ") + std::to_string(_spaceship->GetWeaponCount()));
                }
            }
        });
      }

    y += w->GetSize().y + 4;
  }
        
}

