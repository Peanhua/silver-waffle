#ifndef SUBSYSTEM_SFX_HH_
#define SUBSYSTEM_SFX_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif
#include "Subsystem.hh"

class MusicPlayer;


class SubsystemSfx : public Subsystem
{
public:
  SubsystemSfx();
  
  bool Start()       override;
  void StopThreads() override;
  void Stop()        override;

  MusicPlayer * GetMusicPlayer() const;
  
private:
  MusicPlayer * _music_player;
};

extern SubsystemSfx * Sounds;

#endif
