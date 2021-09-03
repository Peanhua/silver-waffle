#ifndef SOUND_ENVELOPE_HH_
#define SOUND_ENVELOPE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <vector>


class SoundEnvelope
{
public:
  SoundEnvelope(unsigned int samples_per_second);
  virtual ~SoundEnvelope();
  
  void           Press();
  void           Release(double time);
  virtual bool   IsFinished() const = 0;
  virtual double GetNextAmplitude() = 0;
  
protected:
  unsigned int _samples_per_second;
  unsigned int _sample;
  unsigned int _released_at_sample;
};


class SoundEnvelopeADSR : public SoundEnvelope
{
public:
  SoundEnvelopeADSR(unsigned int samples_per_second, double attack_time, double decay_time, double sustain_level, double release_time);
  
  bool   IsFinished() const override;
  double GetNextAmplitude() override;
  
protected:
  unsigned int _attack_samples;
  unsigned int _decay_samples;
  double       _sustain_level;
  unsigned int _release_samples;

  double Attack();
  double Decay();
  double Sustain();
  double Release();
};

class SoundEnvelopeADHSR : public SoundEnvelopeADSR
{
public:
  SoundEnvelopeADHSR(unsigned int samples_per_second, double attack_time, double decay_time, double hold_time, double sustain_level, double release_time);

  bool   IsFinished() const override;
  double GetNextAmplitude() override;

protected:
  double HoldRelease();
  
private:
  unsigned int _hold_samples;
};

#endif
