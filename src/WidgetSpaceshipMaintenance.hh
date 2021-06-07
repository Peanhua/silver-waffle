#ifndef WIDGET_SPACESHIP_MAINTENANCE_HH_
#define WIDGET_SPACESHIP_MAINTENANCE_HH_

#include "Observer.hh"
#include "Widget.hh"

class ObjectSpaceship;
class UpgradeMaterial;
class SpaceshipUpgrade;
class WidgetUpgradeMaterial;


class WidgetSpaceshipMaintenance : public Widget, Observer<UpgradeMaterial>
{
public:
  WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                             ObjectSpaceship * spaceship, std::vector<UpgradeMaterial *> & materials);
  
private:
  ObjectSpaceship * _spaceship;
  std::vector<WidgetUpgradeMaterial *> _available_material_widgets;

  void OnNotifyUpdate(UpgradeMaterial * material);
};

#endif
