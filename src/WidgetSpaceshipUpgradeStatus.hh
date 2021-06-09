#ifndef WIDGET_SPACESHIP_UPGRADE_STATUS_HH_
#define WIDGET_SPACESHIP_UPGRADE_STATUS_HH_

#include "Widget.hh"

class MeshProgressBar;
class SpaceshipUpgrade;


class WidgetSpaceshipUpgradeStatus : public Widget
{
public:
  WidgetSpaceshipUpgradeStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, SpaceshipUpgrade * upgrade);

  void Draw() const override;

  void               SetUpgrade(SpaceshipUpgrade * upgrade);
  SpaceshipUpgrade * GetUpgrade() const;
  
private:
  SpaceshipUpgrade * _upgrade;
  MeshProgressBar *  _meter;
};

#endif
