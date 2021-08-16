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
#include "MusicPlayer.hh"
#include "SubsystemAssetLoader.hh"
#include "WaveformSID.hh"
#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>


MusicPlayer::MusicPlayer()
  : _thread(nullptr),
    _now_playing(nullptr),
    _current_source(0),
    _next_music(nullptr),
    _random_generator(static_cast<unsigned long>(std::time(nullptr))),
    _rdist(0, 1)
{
  _sources.fill(0);
}


void MusicPlayer::Start()
{
  _thread = new std::jthread([this](std::stop_token st)
  {
    alGenSources(static_cast<ALsizei>(_sources.size()), _sources.data());
    assert(alGetError() == AL_NO_ERROR);
    while(!st.stop_requested())
      {
        auto QueueNextBuffer = [this]()
        {
          _now_playing->FillBackBuffer();
          _now_playing->SwapBuffers();
          auto buffer = _now_playing->GetCurrentBuffer();
          alSourceQueueBuffers(_sources[_current_source], 1, &buffer);
          assert(alGetError() == AL_NO_ERROR);
        };
        
        Waveform * newmus = nullptr;
        {
          std::lock_guard lock(_next_music_mutex);
          if(_next_music)
            {
              newmus = _next_music;
              _next_music = nullptr;
            }
        }
        if(newmus)
          {
            _current_source ^= 1;
            _now_playing = newmus;
            QueueNextBuffer();
            QueueNextBuffer();
            alSourcePlay(_sources[_current_source]);
            assert(alGetError() == AL_NO_ERROR);
            
            alSourceStop(_sources[_current_source ^ 1]);
            assert(alGetError() == AL_NO_ERROR);

            alSourcef(_sources[_current_source ^ 1], AL_GAIN, 0);
            alSourcef(_sources[_current_source], AL_GAIN, 1);
          }

        for(auto s : _sources)
          {
            ALint n;
            alGetSourcei(s, AL_BUFFERS_PROCESSED, &n);
            if(n > 0)
              {
                ALuint buffer;
                alSourceUnqueueBuffers(s, 1, &buffer);
                assert(alGetError() == AL_NO_ERROR);
                if(_now_playing && buffer == _now_playing->GetBackBuffer())
                  QueueNextBuffer();
              }
          }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    assert(alGetError() == AL_NO_ERROR);
    alDeleteSources(static_cast<ALsizei>(_sources.size()), _sources.data());
    assert(alGetError() == AL_NO_ERROR);
    _sources.fill(0);
  });
}


void MusicPlayer::Stop()
{
  _thread->request_stop();
  _thread->join();
}


void MusicPlayer::SetMusicCategory(const std::string & category)
{
  auto json = AssetLoader->LoadJson(std::string(DATADIR) + "/Data/Songs");
  assert(json);
  assert(json->is_object());
  assert((*json)[category].is_array());
  auto catsongs = (*json)[category].array_items();
  for(auto s : catsongs)
    {
      assert(s.is_object());
      auto song = new Song(s["filename"].string_value(), static_cast<unsigned int>(s["tune_id"].int_value()));
      if(song->GetWaveform())
        _songs.push_back(song);
      else
        delete song;
    }
  std::cout << "Loaded category " << category << " with " << _songs.size() << " songs.\n";
  PlayNextSong();
}


void MusicPlayer::PlayNextSong()
{
  if(_songs.size() == 0)
    return;
   
  auto index = static_cast<unsigned int>(_rdist(_random_generator) * static_cast<float>(_songs.size()));
  index = std::clamp(index, 0u, static_cast<unsigned int>(_songs.size() - 1));
  SetMusic(_songs[index]->GetWaveform());
}


void MusicPlayer::SetMusic(Waveform * music)
{
  std::lock_guard lock(_next_music_mutex);
  _next_music = music;
}




MusicPlayer::Song::Song(const std::string & filename, unsigned int tune_id)
  : _filename(filename),
    _tune_id(tune_id),
    _waveform(nullptr)
{
  try
    {
      _waveform = new WaveformSID(_filename, _tune_id);
    }
  catch([[maybe_unused]] const std::exception & e)
    {
    }
}


Waveform * MusicPlayer::Song::GetWaveform() const
{
  return _waveform;
}

