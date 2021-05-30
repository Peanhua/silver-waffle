#ifndef WIDGET_WEAPON_STATUS_HH_
#define WIDGET_WEAPON_STATUS_HH_

#include "WidgetSpaceshipStatus.hh"


class WidgetWeaponStatus : public WidgetSpaceshipStatus
{
public:
  WidgetWeaponStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);
  
  void Draw() const override;
};


#endif

