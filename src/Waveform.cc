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
#include <cassert>
#include <iostream>


Waveform::Waveform()
  : _current_buffer(0),
    _length(0)
{
  _buffers.fill(0);

  assert(alGetError() == AL_NO_ERROR);
  alGenBuffers(static_cast<ALsizei>(_buffers.size()), _buffers.data());
  assert(alGetError() == AL_NO_ERROR);
}


Waveform::~Waveform()
{
  alDeleteBuffers(static_cast<ALsizei>(_buffers.size()), _buffers.data());
}


ALuint Waveform::GetCurrentBuffer() const
{
  return _buffers[_current_buffer];
}

ALuint Waveform::GetBackBuffer() const
{
  return _buffers[_current_buffer ^ 1];
}


void Waveform::UpdateBackBuffer(const std::vector<short> & data)
{
  auto buffer = _buffers[_current_buffer ^ 1];

  assert(sizeof(short) == 2);
  assert(alGetError() == AL_NO_ERROR);
  alBufferData(buffer, AL_FORMAT_MONO16, data.data(), static_cast<ALsizei>(data.size() * sizeof(short)), 44100);
  assert(alGetError() == AL_NO_ERROR);
}


void Waveform::SwapBuffers()
{
  _current_buffer ^= 1;
}


double Waveform::GetLength() const
{
  return _length;
}


void Waveform::SetLength(double length)
{
  _length = length;
}
