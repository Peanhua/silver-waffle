#ifndef WIDGET_TELETYPER_HH_
#define WIDGET_TELETYPER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Widget.hh"


class WidgetTeletyper : public Widget
{
public:
  WidgetTeletyper(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);

  void Tick(double deltatime) override;
  void SetText(const std::string & text) override;
  void AppendText(const std::string & text);
  void SetCharactersPerSecond(double characters_per_second);
  void SetPurgingTime(double time_to_purge_line0);
  double GetTime() const; // negative when still typing, positive after typing has finished
  void SetPaused(bool paused);

private:
  std::string  _teletypertext;
  double       _characters_per_second;
  bool         _paused;
  unsigned int _cursor_position;
  double       _timer;
  double       _time_finished;

  double       _time_to_purge_line0;
  double       _purging_timer;
};

#endif
