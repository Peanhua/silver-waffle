#ifndef WIDGET_PLAYER_SHIP_HH_
#define WIDGET_PLAYER_SHIP_HH_

#include "glm.hh"
#include "Widget.hh"

class Mesh;
class TextureRenderer;


class WidgetPlayerShip : public Widget
{
public:
  WidgetPlayerShip(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);
  ~WidgetPlayerShip();
  
  void Tick(double deltatime) override;


private:
  Mesh *            _mesh;
  TextureRenderer * _texture_renderer;
};

#endif
