/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "SoundEnvelope.hh"
#include <cassert>
#include <cmath>

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

SoundEnvelope::SoundEnvelope(unsigned int samples_per_second)
  : _samples_per_second(samples_per_second),
    _sample(0),
    _released_at_sample(0)
{
}


SoundEnvelope::~SoundEnvelope()
{
}


void SoundEnvelope::Press()
{
  _sample = 0;
  _released_at_sample = 0;
}


void SoundEnvelope::Release(double time)
{
  assert(time > 0);
  _released_at_sample = static_cast<unsigned int>(time * static_cast<double>(_samples_per_second));
}


SoundEnvelopeADSR::SoundEnvelopeADSR(unsigned int samples_per_second, double attack_time, double decay_time, double sustain_level, double release_time)
  : SoundEnvelope(samples_per_second),
    _attack_samples(static_cast<unsigned int>(samples_per_second * attack_time)),
    _decay_samples(static_cast<unsigned int>(samples_per_second * decay_time)),
    _sustain_level(sustain_level),
    _release_samples(static_cast<unsigned int>(samples_per_second * release_time))
{
}


bool SoundEnvelopeADSR::IsFinished() const
{
  if(_released_at_sample > 0 && _sample < _released_at_sample)
    return false;
  
  return _sample - _released_at_sample >= _release_samples;
}


double SoundEnvelopeADSR::GetNextAmplitude()
{
  double amplitude;
  if(_sample < _attack_samples)
    amplitude = Attack();
  else if(_sample < _attack_samples + _decay_samples)
    amplitude = Decay();
  else if(_released_at_sample == 0 || _sample < _released_at_sample)
    amplitude = Sustain();
  else
    amplitude = Release();

  _sample++;

  return amplitude;
}

double SoundEnvelopeADSR::Attack()
{ // From 0 to 1
#ifdef DEBUG
  std::cout << "a";
#endif
  double pos = static_cast<double>(_sample) / static_cast<double>(_attack_samples);
  assert(pos >= 0);
  assert(pos <= 1);
  return pos;
}

double SoundEnvelopeADSR::Decay()
{ // From 1 to _sustain_level
#ifdef DEBUG
  std::cout << "d";
#endif
  double pos = static_cast<double>(_sample - _attack_samples) / static_cast<double>(_decay_samples);
  assert(pos >= 0);
  assert(pos <= 1);
  return 1.0 - pos * (1.0 - _sustain_level);
}

double SoundEnvelopeADSR::Sustain()
{ // At _sustain_level
#ifdef DEBUG
  std::cout << "s";
#endif
  return _sustain_level;
}

double SoundEnvelopeADSR::Release()
{ // From _sustain_level to 0
#ifdef DEBUG
  std::cout << "r";
#endif
  auto released_at_sample = std::max(_released_at_sample, _attack_samples + _decay_samples);
  double pos = static_cast<double>(_sample - released_at_sample) / static_cast<double>(_release_samples);
  assert(pos >= 0);
  assert(pos <= 1);
  return std::lerp(_sustain_level, 0.0, std::clamp(pos, 0.0, 1.0));
}




SoundEnvelopeADHSR::SoundEnvelopeADHSR(unsigned int samples_per_second, double attack_time, double decay_time, double hold_time, double sustain_level, double release_time)
  : SoundEnvelopeADSR(samples_per_second, attack_time, decay_time, sustain_level, release_time),
    _hold_samples(static_cast<unsigned int>(samples_per_second * hold_time))
{
}


double SoundEnvelopeADHSR::GetNextAmplitude()
{
#ifdef DEBUG
  std::cout << _sample << " ";
#endif
  double amplitude;
  if(_sample < _attack_samples)
    amplitude = Attack();
  else if(_sample < _attack_samples + _decay_samples)
    amplitude = Decay();
  else if(_sample < _attack_samples + _decay_samples + _hold_samples)
    amplitude = Sustain();
  else
    amplitude = HoldRelease();

#ifdef DEBUG
  std::cout << " " << amplitude << std::endl;
#endif
  _sample++;

  return amplitude;
}


double SoundEnvelopeADHSR::HoldRelease()
{
#ifdef DEBUG
  std::cout << "r";
#endif
  auto released_at_sample = _attack_samples + _decay_samples + _hold_samples;
  double pos = static_cast<double>(_sample - released_at_sample) / static_cast<double>(_release_samples);
  return std::lerp(_sustain_level, 0.0, std::clamp(pos, 0.0, 1.0));
}


bool SoundEnvelopeADHSR::IsFinished() const
{
  return _sample > _attack_samples + _decay_samples + _hold_samples + _release_samples;
}
