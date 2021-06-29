#ifndef WIDGET_BUTTON_HH_
#define WIDGET_BUTTON_HH_

#include "Widget.hh"

class Mesh;


class WidgetButton : public Widget
{
public:
  WidgetButton(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);
  ~WidgetButton();

  void Draw() const override;
  
private:
  Mesh * _mesh_up;
  Mesh * _mesh_down;
};

#endif
