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
  void FadeOutCurrentSong(double time);

private:
  const std::chrono::steady_clock _clock;
  std::jthread * _thread;
  Waveform *     _now_playing;
  bool           _now_playing_continuously;
  double         _volume;
  std::chrono::time_point<std::chrono::steady_clock> _playing_stop_time;
  double         _fading_out;
  double         _fading_out_volume;
  
  std::array<ALuint, 2> _sources;
  unsigned int          _current_source;

  std::atomic<Waveform *> _next_music;
  std::mutex              _next_music_mutex;

  class Song
  {
  public:
    Song(const std::string & filename, unsigned int tune_id, double length);
    Waveform * GetWaveform() const;
  private:
    std::string  _filename;
    unsigned int _tune_id;
    Waveform *   _waveform;
  };
  std::vector<Song *> _songs;

  std::mt19937_64                       _random_generator;
  std::uniform_real_distribution<float> _rdist;

  Waveform * GetNextSongInCategory();

  void TickMusicChange();
  void ChangeMusic(Waveform * music);
  void SetNextMusic(Waveform * music);

  void TickSourceQueues();
  void QueueNextBuffer();

  void TickFading();
};

#endif
