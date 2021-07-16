/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemHandler.hh"
#include <cassert>
#include <iostream>


bool SubsystemHandler::StartAll()
{
  bool rv = true;
  
  for(auto it = _subsystems.begin(); rv == true && it != _subsystems.end(); it++)
    {
      assert(*it);
      std::cout << "Starting subsystem: " << (*it)->GetName() << std::endl;
      rv = (*it)->Start();
      if(!rv)
        std::cout << "Failed to start subsystem " << (*it)->GetName() << std::endl;
    }
  
  return rv;
}


void SubsystemHandler::StopAll()
{
  for(auto it = _subsystems.rbegin(); it != _subsystems.rend(); it++)
    (*it)->Stop();
}


void SubsystemHandler::Add(Subsystem * subsystem)
{
  assert(subsystem);
  _subsystems.push_back(subsystem);
}


void SubsystemHandler::PreTickAll()
{
  for(auto s : _subsystems)
    s->PreTick();
}


void SubsystemHandler::TickAll(double deltatime)
{
  for(auto s : _subsystems)
    s->Tick(deltatime);
}


void SubsystemHandler::PostTickAll()
{
  for(auto s : _subsystems)
    s->PostTick();
}


bool SubsystemHandler::IsRunning() const
{
  bool rv = true;
  for(auto s : _subsystems)
    if(!s->IsRunning())
      rv = false;
  return rv;
}
