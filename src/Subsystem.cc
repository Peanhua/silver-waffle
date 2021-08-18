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

Subsystem::Subsystem(const std::string & name)
  : _name(name)
{
}


Subsystem::~Subsystem()
{
}


void Subsystem::StopThreads()
{
}


const std::string & Subsystem::GetName() const
{
  return _name;
}


void Subsystem::PreTick()
{
}


void Subsystem::Tick([[maybe_unused]] double deltatime)
{
}


void Subsystem::PostTick()
{
}


bool Subsystem::IsRunning() const
{
  return true;
}



