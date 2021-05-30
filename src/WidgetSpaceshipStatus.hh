#ifndef WIDGET_SPACESHIP_STATUS_HH_
#define WIDGET_SPACESHIP_STATUS_HH_

#include "Widget.hh"

class MeshProgressBar;
class ObjectSpaceship;


class WidgetSpaceshipStatus : public Widget
{
public:
  WidgetSpaceshipStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);

  void Draw() const override;

  void              SetSpaceship(ObjectSpaceship * spaceship);
  ObjectSpaceship * GetSpaceship() const;

protected:
  ObjectSpaceship * _spaceship;
  MeshProgressBar * _meter;
};


#endif

