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
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include <alut.h>
#include <cassert>
#include <iostream>
#include <libfmsynth/Blueprint.hh>
#include <libfmsynth/NodeAudioDeviceOutput.hh>


SubsystemSfx * Sounds = nullptr;


SubsystemSfx::SubsystemSfx()
  : Subsystem("Sfx"),
    _music_player(nullptr),
    _sfx_volume(0),
    _sfx_generator_thread(nullptr),
    _sfx_generator_ready(false),
    _next_sfx_source(0)
{
}


bool SubsystemSfx::Start()
{
  assert(!Sounds);

  bool enabled = Settings->GetBool("music") || Settings->GetBool("sfx");
  std::cout << GetName() << ": Audio is " << (enabled ? "enabled" : "disabled") << ".\n";

  bool success = true;
  if(enabled)
    success = alutInit(nullptr, nullptr);
  if(!success)
    return false;

  _music_player = new MusicPlayer();
  assert(_music_player);
  _music_player->Start();

  _sfx_volume = static_cast<ALfloat>(Settings->GetDouble("sfx_volume") / 100.0);
  {
    ALfloat orientation[] = {
      0, 1, 0,
      0, 0, 1
    };
    alListenerfv(AL_ORIENTATION, orientation);
    //alListenerf(AL_GAIN, static_cast<ALfloat>());
  }
  for(int i = 0; i < 128; i++)
    {
      ALuint s;
      alGenSources(1, &s);
      alSourcef(s, AL_MAX_DISTANCE, 50);
      alSourcef(s, AL_REFERENCE_DISTANCE, 10);
      _sound_effect_sources.push_back(s);
    }

  if(!_sfx_generator_ready && !_sfx_generator_thread)
    _sfx_generator_thread = new std::jthread([this](std::stop_token st)
    {
      _sound_effects.clear();
      for(unsigned int i = 0; i < _ids.size(); i++)
        _sound_effects.push_back(AL_NONE);

      auto it = _ids.cbegin();
      std::vector<int16_t> buffer;

      while(!st.stop_requested() && it != _ids.cend())
        {
          buffer.clear();

          auto id = *it;
          {
            fmsynth::Blueprint blueprint;
            auto json = AssetLoader->LoadJson("Sounds/" + std::string(id) + ".sbp");
            auto loadok = blueprint.Load(*json);
            if(loadok)
              {
                auto ados = blueprint.GetNodesByType("AudioDeviceOutput");
                if(ados.size() > 0)
                  {
                    auto node = dynamic_cast<fmsynth::NodeAudioDeviceOutput *>(ados[0]);
                    if(node)
                      {
                        node->SetOnPlaySample([&buffer, &node](double sample)
                        {
                          buffer.push_back(node->SampleToInt(sample));
                        });
                        
                        bool done = false;
                        while(!done)
                          {
                            std::lock_guard lock(blueprint.GetLockMutex());
                            if(blueprint.IsFinished())
                              done = true;
                            else
                              blueprint.Tick(1);
                          }
                      }
                  }
              }

            if(buffer.size() > 0)
              {
                ALuint alb;
                
                alGenBuffers(1, &alb);
                alBufferData(alb, AL_FORMAT_MONO16, buffer.data(), static_cast<ALsizei>(buffer.size() * sizeof(int16_t)), 44100);
                assert(alGetError() == AL_NO_ERROR);
                
                _sound_effects[GetSoundEffectIndex(id)] = alb;
                
                std::cout << (GetName() + ": Generated sound effect '" + std::string(id) + "' (" + std::to_string(buffer.size()) + " samples).\n") << std::flush;
              }
            else
              std::cerr << GetName() << ": Failed to generate sound effect '" << std::string(id) << "': No data." << std::endl;
          }
          
          it++;
        }
        
      if(it == _ids.cend())
        _sfx_generator_ready = true;
    });

  Sounds = this;

  return success;
}


void SubsystemSfx::StopThreads()
{
  if(_music_player)
    _music_player->Stop();
  if(_sfx_generator_thread)
    {
      _sfx_generator_thread->request_stop();
      _sfx_generator_thread->join();
    }
}


void SubsystemSfx::Stop()
{
  if(Sounds == this)
    Sounds = nullptr;

  delete _music_player;
  _music_player = nullptr;

  alutExit();
}


MusicPlayer * SubsystemSfx::GetMusicPlayer() const
{
  return _music_player;
}


void SubsystemSfx::PlaySoundEffect(ALuint soundbuffer, const glm::vec3 & position)
{
  if(!_sfx_generator_ready)
    return;
  
  auto source = _sound_effect_sources[_next_sfx_source];

  _next_sfx_source++;
  if(_next_sfx_source >= _sound_effect_sources.size())
    _next_sfx_source = 0;
  
  alSourcef(source, AL_GAIN, 0);
  alSourceStop(source);
  alSourcefv(source, AL_POSITION, glm::value_ptr(position));
  alSourcei(source, AL_BUFFER, static_cast<ALint>(soundbuffer));
  alSourcePlay(source);
  alSourcef(source, AL_GAIN, _sfx_volume);
}

