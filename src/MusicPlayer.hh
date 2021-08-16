#ifndef MUSIC_PLAYER_HH_
#define MUSIC_PLAYER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <array>
#include <thread>
#include <al.h>

class Waveform;


class MusicPlayer
{
public:
  MusicPlayer();

  void Start();
  void Stop();
  void SetMusic(Waveform * music);

private:
  std::jthread * _thread;
  Waveform *     _now_playing;
  std::array<ALuint, 2> _sources;
  unsigned int          _current_source;

  std::atomic<Waveform *> _next_music;
  std::mutex              _next_music_mutex;
};

#endif
