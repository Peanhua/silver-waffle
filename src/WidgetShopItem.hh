#ifndef WIDGET_SHOP_ITEM_HH_
#define WIDGET_SHOP_ITEM_HH_

#include "Widget.hh"

class ObjectSpaceship;
class SpaceshipUpgrade;
class UpgradeMaterial;


class WidgetShopItem : public Widget
{
public:
  WidgetShopItem(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                 ObjectSpaceship * spaceship,
                 std::vector<UpgradeMaterial *> & available_materials,
                 SpaceshipUpgrade * upgrade);
  
private:
  ObjectSpaceship *                _spaceship;
  std::vector<UpgradeMaterial *> & _available_materials;
  SpaceshipUpgrade *               _upgrade;

  bool CanBuy() const;
};

#endif
