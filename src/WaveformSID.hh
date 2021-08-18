#ifndef WAVEFORM_SID_HH_
#define WAVEFORM_SID_HH_
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
#include <sidplayfp/sidplayfp.h>

class WaveformSID : public Waveform
{
public:
  WaveformSID(const std::string & filename, unsigned int tune_id);

  void Restart()        override;
  void FillBackBuffer() override;

private:
  std::vector<short> _data;
  sidplayfp          _sid;
  SidTune *          _tune;
};


#endif
