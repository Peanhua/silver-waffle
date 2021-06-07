#ifndef WIDGET_WEAPON_STATUS_HH_
#define WIDGET_WEAPON_STATUS_HH_

#include "WidgetSpaceshipStatus.hh"


class WidgetWeaponStatus : public WidgetSpaceshipStatus
{
public:
  WidgetWeaponStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship);
  
  void Tick(double deltatime) override;
  void Draw() const override;
  void SetSpaceship(ObjectSpaceship * spaceship) override;

private:
  unsigned int _weaponcount;
  
  void UpdateMeter(bool force = false);
};


#endif

