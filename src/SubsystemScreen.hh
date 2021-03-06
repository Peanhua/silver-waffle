#ifndef SUBSYSTEM_GAME_STATE_HH_
#define SUBSYSTEM_GAME_STATE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Subsystem.hh"
#include <vector>

class Screen;

class SubsystemScreen : public Subsystem
{
public:
  SubsystemScreen();
  
  bool Start() override;
  void Stop() override;
  void Tick(double deltatime) override;
  bool IsRunning() const override;

  Screen * GetScreen() const;
  
private:
  Screen * _root_screen;
};

extern SubsystemScreen * ScreenManager;

#endif

