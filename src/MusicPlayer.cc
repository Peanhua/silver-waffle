/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "MusicPlayer.hh"
#include "Waveform.hh"
#include <cassert>
#include <chrono>
#include <iostream>


MusicPlayer::MusicPlayer()
  : _thread(nullptr),
    _now_playing(nullptr),
    _current_source(0),
    _next_music(nullptr)
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


void MusicPlayer::SetMusic(Waveform * music)
{
  std::lock_guard lock(_next_music_mutex);
  _next_music = music;
}
