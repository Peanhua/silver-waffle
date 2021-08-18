/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "WaveformSID.hh"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/builders/residfp.h>


WaveformSID::WaveformSID(const std::string & filename, unsigned int tune_id)
  : Waveform()
{
  auto rs = new ReSIDfpBuilder("SilverWaffle");
  auto maxsids = _sid.info().maxsids();
  rs->create(maxsids);
  if(!rs->getStatus())
    throw std::runtime_error(rs->error());

  auto tune = new SidTune(filename.c_str());
  if(!tune->getStatus())
    {
      auto path = std::getenv("HVSC_BASE");
      if(path)
        tune = new SidTune((std::string(path) + "/" + filename).c_str());
    }
  if(!tune->getStatus())
    throw std::runtime_error(tune->statusString());
 
  tune->selectSong(tune_id);
 
  static SidConfig cfg;
  cfg.frequency      = 44100;
  cfg.samplingMethod = SidConfig::INTERPOLATE;
  cfg.fastSampling   = false;
  cfg.playback       = SidConfig::MONO;
  cfg.sidEmulation   = rs;
  if(!_sid.config(cfg))
    throw std::runtime_error(_sid.error());
 
  if(!_sid.load(tune))
    throw std::runtime_error(_sid.error());

  _data.resize(cfg.frequency);
}


void WaveformSID::Restart()
{
  _sid.stop();
}


void WaveformSID::FillBackBuffer()
{
  _sid.play(_data.data(), 44100);
  UpdateBackBuffer(_data);
}
