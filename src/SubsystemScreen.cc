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
  _root_screen->PlayMusic();
  
  return GetScreen();
}


void SubsystemScreen::Stop()
{
  while(auto screen = GetScreen())
    screen->Quit();
  
  if(ScreenManager == this)
    ScreenManager = nullptr;

  delete _root_screen;
  _root_screen = nullptr;
}


void SubsystemScreen::Tick(double deltatime)
{
  auto screen = GetScreen();
  if(!screen)
    return;

  auto child = screen->GetChild();
  if(child && !child->IsRunning())
    {
      screen->SetChild(nullptr);
      delete child;
      screen->PlayMusic();
    }

  screen->Tick(deltatime);
}


bool SubsystemScreen::IsRunning() const
{
  auto screen = GetScreen();
  if(!screen)
    return false;

  return screen->IsRunning();
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

