#ifndef OSCILLATOR_HH_
#define OSCILLATOR_HH_
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
#include <numbers>
#include <random>


class Oscillator
{
public:
  static constexpr double FrequencyToAngularVelocity(double frequency)
  {
    return std::numbers::pi * 2.0 * frequency;
  };

  Oscillator()
    : _frequency(0),
      _velocity(0),
      _vibrato_velocity(0),
      _vibrato_amplitude(0)
  {
  }
    
  Oscillator(double frequency)
    : _frequency(frequency),
      _velocity(FrequencyToAngularVelocity(frequency)),
      _vibrato_velocity(0),
      _vibrato_amplitude(0)
  {
  }

  virtual ~Oscillator()
  {
  }
  
  virtual double Get(double time) = 0;

  void SetVibrato(double frequency, double amplitude)
  {  // todo: add envelope for vibrato
    _vibrato_velocity = FrequencyToAngularVelocity(frequency);
    _vibrato_amplitude = amplitude;
  }

protected:
  double _frequency;
  double _velocity;
  double _vibrato_velocity;
  double _vibrato_amplitude;
  
  double GetVelocity(double time) const
  {
    double rv = _velocity * time;
    
    if(_vibrato_amplitude > 0)
      rv += _vibrato_amplitude * _frequency * sin(_vibrato_velocity * time);

    return rv;
  }
};


class OscillatorSine : public Oscillator
{
public:
  using Oscillator::Oscillator;

  double Get(double time) override
  {
    return std::sin(GetVelocity(time));
  };
};


class OscillatorSquare : public OscillatorSine
{
public:
  using OscillatorSine::OscillatorSine;
  
  double Get(double time) override
  {
    return OscillatorSine::Get(time) < 0 ? -1 : 1;
  };
};


class OscillatorTriangle : public Oscillator
{
public:
  using Oscillator::Oscillator;

  double Get(double time) override
  {
    return 2.0 / std::numbers::pi * std::asin(std::sin(GetVelocity(time)));
  };
};


class OscillatorSawtooth : public Oscillator
{
public:
  using Oscillator::Oscillator;

  double Get(double time) override
  {
    double rv = 0;
    for(double i = 1; i < 12; i++)
      rv += std::sin(i * GetVelocity(time)) / i;
    return rv * 2.0 / std::numbers::pi;
  };
};


class OscillatorNoise : public Oscillator
{
public:
  OscillatorNoise(unsigned int random_seed)
    : Oscillator(),
      _random_generator(random_seed),
      _rdist(0, 1)
  {
  }
  
  double Get([[maybe_unused]] double time) override
  {
    return _rdist(_random_generator);
  }

private:
  std::mt19937_64                        _random_generator;
  std::uniform_real_distribution<double> _rdist;
};



#endif
