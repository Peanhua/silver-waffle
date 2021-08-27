/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "SubsystemSfx.hh"
#include "MusicPlayer.hh"
#include "SubsystemSettings.hh"
#include <alut.h>
#include <cassert>
#include <iostream>


SubsystemSfx * Sounds = nullptr;


SubsystemSfx::SubsystemSfx()
  : Subsystem("Sfx"),
    _music_player(nullptr)
{
}


bool SubsystemSfx::Start()
{
  assert(!Sounds);

  bool enabled = Settings->GetBool("music");
  std::cout << GetName() << ": Audio is " << (enabled ? "enabled" : "disabled") << ".\n";

  bool success = true;
  if(enabled)
    success = alutInit(nullptr, nullptr);
  if(!success)
    return false;

  _music_player = new MusicPlayer();
  assert(_music_player);

  _music_player->Start();

  Sounds = this;

  return success;
}


void SubsystemSfx::StopThreads()
{
  if(_music_player)
    _music_player->Stop();
}


void SubsystemSfx::Stop()
{
  if(Sounds == this)
    Sounds = nullptr;

  delete _music_player;

  alutExit();
}


MusicPlayer * SubsystemSfx::GetMusicPlayer() const
{
  return _music_player;
}

