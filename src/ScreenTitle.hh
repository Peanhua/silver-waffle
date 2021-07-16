#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Screen.hh"
#include <vector>

class Camera;
class Milkyway;
class Object;
class SpaceParticles;
class WidgetTeletyper;


class ScreenTitle : public Screen
{
public:
  ScreenTitle();
  ~ScreenTitle();
  
  void Tick(double deltatime) override;
  void TickPlot();
  
private:
  double           _time;
  Milkyway *       _milkyway;
  SpaceParticles * _particles;
  Camera *         _camera;
  std::vector<Object *> _planets;
  WidgetTeletyper * _teletyper;
  unsigned int      _plot_phase;
  Widget * _demo_start_button;
};

#endif
