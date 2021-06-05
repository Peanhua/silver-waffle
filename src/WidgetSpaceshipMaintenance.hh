#ifndef WIDGET_SPACESHIP_MAINTENANCE_HH_
#define WIDGET_SPACESHIP_MAINTENANCE_HH_

#include "Widget.hh"

class ObjectSpaceship;


class WidgetSpaceshipMaintenance : public Widget
{
public:
  typedef std::function<unsigned int(unsigned int)>       get_material_amount_t;
  typedef std::function<void(unsigned int, unsigned int)> use_material_t;
  
  WidgetSpaceshipMaintenance(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship,
                             get_material_amount_t GetMaterialAmount,
                             use_material_t        UseMaterial);

private:
  ObjectSpaceship * _spaceship;
};

#endif
