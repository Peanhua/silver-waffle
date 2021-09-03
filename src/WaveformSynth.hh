#ifndef WAVEFORM_SYNTH_HH_
#define WAVEFORM_SYNTH_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Waveform.hh"
#include "Oscillator.hh"
#include <cstdint>
#include <iostream>
#include <vector>


class WaveformSynth : public Waveform
{
public:
  WaveformSynth()
    : Waveform(nullptr),
      _time(0),
      _tremolo_amplitude(0),
      _tremolo_velocity(0)
  {
    _data.resize(44100 / 10);
  }

  void Restart() override
  {
    _time = 0;
  }
  
  void FillBackBuffer() override
  {
    for(unsigned int i = 0; i < _data.size(); i++)
      {
        double t = _time + static_cast<double>(i) / 44100.0;
        auto s = GetSample(t);
        // No master volume envelope is being applied here, because UpdateBackBuffer() applies it.
        _data[i] = SampleToInt(s);
      }

    _time += static_cast<double>(_data.size()) / 44100.0;
    UpdateBackBuffer(_data);
  }

  double GetSample(double time)
  {
    return GetOscillatorSample(time) * Tremolo(time);
  }

  int16_t SampleToInt(double sample)
  {
    if(sample < 0)
      sample *= static_cast<double>(std::abs(INT16_MIN));
    else
      sample *= static_cast<double>(INT16_MAX);

    return static_cast<int16_t>(sample);
  }
  

protected:
  constexpr double AdjustOctave(double frequency, double octave_adjustment)
  {
    return frequency * std::pow(2.0, octave_adjustment / 12.0);
  }

  void SetTremolo(double frequency, double amplitude)
  {
    _tremolo_amplitude = amplitude;
    _tremolo_velocity = Oscillator::FrequencyToAngularVelocity(frequency);
  }

  Oscillator * AddOscillator(double weight, Oscillator * oscillator)
  {
    _oscillators.push_back(oscillator);
    _weights.push_back(weight);
    return oscillator;
  }
  
private:
  std::vector<Oscillator *> _oscillators;
  std::vector<double>       _weights;
  double  _time;
  double  _tremolo_amplitude;
  double  _tremolo_velocity;
  std::vector<int16_t> _data;

  double GetOscillatorSample(double time) const
  {
    double rv = 0;
    for(unsigned int i = 0; i < _oscillators.size(); i++)
      rv += _weights[i] * _oscillators[i]->Get(time);
    return rv;
  }

  double Tremolo(double time)
  {
    if(_tremolo_amplitude > 0)
      return (1.0 - _tremolo_amplitude) + _tremolo_amplitude * std::sin(_tremolo_velocity * time);
    else
      return 1;
  }
};



class WaveformSynthPlasmaWeapon : public WaveformSynth
{
public:
  WaveformSynthPlasmaWeapon()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.05, 0.05, 0.5, 0.1));
    auto o = AddOscillator(0.2, new OscillatorSawtooth(2000));
    o->SetVibrato(5.0, 0.1);
  }
};


class WaveformSynthExplosionBig : public WaveformSynth
{
public:
  WaveformSynthExplosionBig()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.1, 0.2, 0.3, 1.0));
    AddOscillator(0.75, new OscillatorNoise(0));
    AddOscillator(0.25, new OscillatorSquare(15));
  }
};


class WaveformSynthExplosionSmall : public WaveformSynth
{
public:
  WaveformSynthExplosionSmall()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.1, 0.2, 0.1, 0.5));
    AddOscillator(0.5, new OscillatorNoise(0));
  }
};


class WaveformSynthExplosionTiny : public WaveformSynth
{
public:
  WaveformSynthExplosionTiny()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.1, 0.1, 0.1, 0.2));
    AddOscillator(0.25, new OscillatorNoise(0));
  }
};


class WaveformSynthWeoew : public WaveformSynth
{
public:
  WaveformSynthWeoew()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.2, 0.2, 0.1, 0.4, 0.1));
    auto o = AddOscillator(0.75, new OscillatorSawtooth(3200));
    o->SetVibrato(2.0, 0.05);
    SetTremolo(10, 1);
  }
};


class WaveformSynthCollectValuable : public WaveformSynth
{
public:
  WaveformSynthCollectValuable()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.05, 0.05, 0.5, 0.3));
    AddOscillator(0.2, new OscillatorSine(440*10));
    AddOscillator(0.05, new OscillatorSine(440*9));
    SetTremolo(10, 1);
  }
};


class WaveformSynthDropBomb : public WaveformSynth
{
public:
  WaveformSynthDropBomb()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.05, 0.5, 1.0, 1.0));
    auto o = AddOscillator(0.33, new OscillatorTriangle(440*4));
    o->SetVibrato(0.25, 1.0);
  }
};


class WaveformSynthEngine : public WaveformSynth
{
public:
  WaveformSynthEngine()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.05, 0.1, 1.0, 0.05));
    AddOscillator(0.02, new OscillatorNoise(300));
    SetTremolo(90, 1);
  }
};


class WaveformSynthTest : public WaveformSynth
{
public:
  WaveformSynthTest()
    : WaveformSynth()
  {
    SetEnvelope(new SoundEnvelopeADHSR(44100, 0.01, 0.05, 0.3, 1.0, 0.05));
    //SetEnvelope(new SoundEnvelopeADHSR(44100, 1, 1, 1, 1, 1));
    auto o = AddOscillator(0.25, new OscillatorNoise(300));
    o->SetVibrato(10, 0.2);
    //AddOscillator(0.25, new OscillatorSawtooth(1600));
    //AddOscillator(0.25 / 1.75, new OscillatorSine(base_frequency * 3));
    SetTremolo(20, 1);
  }
};

#endif
