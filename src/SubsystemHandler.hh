#ifndef SUBSYSTEM_HANDLER_HH_
#define SUBSYSTEM_HANDLER_HH_
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

class SubsystemHandler
{
public:
  bool StartAll();
  void StopAll();
  void Add(Subsystem * subsystem);
  void PreTickAll();
  void TickAll(double deltatime);
  void PostTickAll();
  bool IsRunning() const;

private:
  std::vector<Subsystem *> _subsystems;
};

#endif
