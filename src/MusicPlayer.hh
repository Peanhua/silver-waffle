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
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <al.h>

class Waveform;


class MusicPlayer
{
public:
  MusicPlayer();

  void Start();
  void Stop();
  void SetMusicCategory(const std::string & category);

private:
  std::jthread * _thread;
  Waveform *     _now_playing;
  std::array<ALuint, 2> _sources;
  unsigned int          _current_source;

  std::atomic<Waveform *> _next_music;
  std::mutex              _next_music_mutex;

  class Song
  {
  public:
    Song(const std::string & filename, unsigned int tune_id);
    Waveform * GetWaveform() const;
  private:
    std::string  _filename;
    unsigned int _tune_id;
    Waveform *   _waveform;
  };
  std::vector<Song *> _songs;

  std::mt19937_64                       _random_generator;
  std::uniform_real_distribution<float> _rdist;
  
  void PlayNextSong();
  void SetMusic(Waveform * music);
};

#endif
