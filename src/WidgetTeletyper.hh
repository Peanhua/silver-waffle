#ifndef WIDGET_TELETYPER_HH_
#define WIDGET_TELETYPER_HH_

#include "Widget.hh"


class WidgetTeletyper : public Widget
{
public:
  WidgetTeletyper(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);

  void Tick(double deltatime) override;
  void SetText(const std::string & text) override;
  void SetCharactersPerSecond(double characters_per_second);
  double GetTime() const; // negative when still typing, positive after typing has finished

private:
  std::string  _teletypertext;
  double       _characters_per_second;
  unsigned int _cursor_position;
  double       _timer;
  double       _time_finished;
};

#endif
