#ifndef WAVEFORM_HH_
#define WAVEFORM_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <array>
#include <string>
#include <vector>
#include <al.h>


class Waveform
{
public:
  Waveform();
  virtual ~Waveform();
  
  ALuint       GetCurrentBuffer() const;
  ALuint       GetBackBuffer() const;
  void         SwapBuffers();
  virtual void FillBackBuffer() = 0;

  double       GetLength() const;
  void         SetLength(double length);

protected:
  void UpdateBackBuffer(const std::vector<short> & data);
  
private:
  unsigned int          _current_buffer;
  std::array<ALuint, 2> _buffers;
  double                _length;
};

#endif
