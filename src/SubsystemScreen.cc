/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemScreen.hh"
#include "ScreenTitle.hh"


SubsystemScreen * ScreenManager = nullptr;


SubsystemScreen::SubsystemScreen()
  : Subsystem("Screen"),
    _root_screen(nullptr)
{
}


bool SubsystemScreen::Start()
{
  assert(!ScreenManager);
  ScreenManager = this;
  
  _root_screen = new ScreenTitle();
  return GetScreen();
}


void SubsystemScreen::Stop()
{
  while(auto state = GetScreen())
    state->Quit();
  
  if(ScreenManager == this)
    ScreenManager = nullptr;

  delete _root_screen;
  _root_screen = nullptr;
}


void SubsystemScreen::Tick(double deltatime)
{
  auto state = GetScreen();
  if(!state)
    return;

  auto child = state->GetChild();
  if(child && !child->IsRunning())
    {
      state->SetChild(nullptr);
      delete child;
    }

  state->Tick(deltatime);
}


bool SubsystemScreen::IsRunning() const
{
  auto state = GetScreen();
  if(!state)
    return false;

  return state->IsRunning();
}


Screen * SubsystemScreen::GetScreen() const
{
  auto rv = _root_screen;
  if(rv && !rv->IsRunning())
    return nullptr;

  auto next = rv;
  while(next && next->IsRunning())
    {
      rv = next;
      next = rv->GetChild();
    }

  return rv;
}

