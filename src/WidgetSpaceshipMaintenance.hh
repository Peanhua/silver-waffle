#ifndef WIDGET_SPACESHIP_MAINTENANCE_HH_
#define WIDGET_SPACESHIP_MAINTENANCE_HH_

#include "Widget.hh"

class ObjectSpaceship;
class GameStats;
class UpgradeMaterial;
class SpaceshipUpgrade;


class WidgetSpaceshipMaintenance : public Widget
{
public:
  WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                             ObjectSpaceship * spaceship, GameStats * gamestats);
  
private:
  ObjectSpaceship * _spaceship;
};

#endif
