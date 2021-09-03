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
#include "GLM.hh"
#include "Subsystem.hh"
#include <al.h>
#include <array>
#include <thread>
#include <vector>

class MusicPlayer;


class SubsystemSfx : public Subsystem
{
public:
  SubsystemSfx();
  
  bool Start()       override;
  void StopThreads() override;
  void Stop()        override;

  MusicPlayer * GetMusicPlayer() const;

  inline void PlaySoundEffect(const std::string_view & name, const glm::vec3 & position)
  {
    PlaySoundEffect(_sound_effects[GetSoundEffectIndex(name)], position);
  }
  
private:
  MusicPlayer *  _music_player;
  ALfloat        _sfx_volume;
  std::jthread * _sfx_generator_thread;
  bool           _sfx_generator_ready;
  std::vector<ALuint> _sound_effects;
  std::vector<ALuint> _sound_effect_sources;
  unsigned int        _next_sfx_source;

  static constexpr unsigned int _sfx_count = 8;


  constexpr unsigned int GetSoundEffectIndex(const std::string_view & name) const
  {
    std::array<std::string_view, _sfx_count> names
      {
        "alien",
        "collect.valuable",
        "explosion.big",
        "explosion.small",
        "explosion.tiny",
        "drop_bomb",
        "engine",
        "weapon"
      };
    
    for(unsigned int i = 0; i < names.size(); i++)
      if(name == names[i])
        return i;
    return 0;
  }

  void PlaySoundEffect(ALuint soundbuffer, const glm::vec3 & position);
};

extern SubsystemSfx * Sounds;

#endif
