#ifndef WIDGET_HIGHSCORES_HH_
#define WIDGET_HIGHSCORES_HH_

#include "Widget.hh"

class HighscoreEntry;


class WidgetHighscores : public Widget
{
public:
  WidgetHighscores(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);

  void Tick(double deltatime) override;

  void Refresh();
  
private:
  HighscoreEntry * _hilited;
  unsigned int     _last_refresh;
};

#endif
