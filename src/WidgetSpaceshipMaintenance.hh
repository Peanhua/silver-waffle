#ifndef WIDGET_SPACESHIP_MAINTENANCE_HH_
#define WIDGET_SPACESHIP_MAINTENANCE_HH_

#include "Widget.hh"

class ObjectSpaceship;
class UpgradeMaterial;


class WidgetSpaceshipMaintenance : public Widget
{
public:
  WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                             ObjectSpaceship * spaceship, std::vector<UpgradeMaterial *> & materials);

private:
  ObjectSpaceship * _spaceship;
};

#endif
