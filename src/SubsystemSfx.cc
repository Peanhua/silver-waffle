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
#include "WaveformSynth.hh"
#include <alut.h>
#include <cassert>
#include <iostream>


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

  _sfx_volume = Settings->GetDouble("sfx_volume") / 100.0;
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
      for(unsigned int i = 0; i < _sfx_count; i++)
        _sound_effects.push_back(AL_NONE);

      auto json = AssetLoader->LoadJson("Data/Sfx");
      assert(json);
      assert(json->is_object());
      auto effects = (*json)["effects"].array_items();
      auto it = effects.cbegin();
      std::vector<int16_t> buffer;
        
      while(!st.stop_requested() && it != effects.cend())
        {
          assert((*it)["id"].is_string());
          auto id = (*it)["id"].string_value();
          {
            WaveformSynth * effect;
            
            if(id == "explosion.big")
              effect = new WaveformSynthExplosionBig();
            else if(id == "explosion.small")
              effect = new WaveformSynthExplosionSmall();
            else if(id == "explosion.tiny")
              effect = new WaveformSynthExplosionTiny();
            else if(id == "weapon")
              effect = new WaveformSynthPlasmaWeapon();
            else if(id == "alien")
              effect = new WaveformSynthWeoew();
            else if(id == "drop_bomb")
              effect = new WaveformSynthDropBomb();
            else if(id == "engine")
              effect = new WaveformSynthEngine();
            else if(id == "human_jump")
              effect = new WaveformSynthHumanJump();
            else if(id == "collect.valuable")
              effect = new WaveformSynthCollectValuable();
            else
              assert(false);

            buffer.clear();
            while(!effect->GetEnvelope()->IsFinished())
              {
                auto t = static_cast<double>(buffer.size()) / 44100.0;
                auto s = effect->GetSample(t);
                auto a = effect->GetEnvelope()->GetNextAmplitude();
                auto w = effect->SampleToInt(s * a);
                buffer.push_back(w);
              }

            delete effect;
          }

          {
            ALuint alb;
              
            alGenBuffers(1, &alb);
            alBufferData(alb, AL_FORMAT_MONO16, buffer.data(), static_cast<ALsizei>(buffer.size() * sizeof(int16_t)), 44100);
            assert(alGetError() == AL_NO_ERROR);

            _sound_effects[GetSoundEffectIndex(id)] = alb;
          }

          std::cout << (GetName() + ": Generated sound effect '" + id + "' (" + std::to_string(buffer.size()) + " samples).\n") << std::flush;
          
          it++;
        }
        
      if(it == effects.cend())
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

