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
#include "SoundEnvelope.hh"
#include <cassert>
#include <iostream>


Waveform::Waveform(SoundEnvelope * envelope)
  : _envelope(envelope),
    _current_buffer(0),
    _length(0)
{
  _buffers.fill(0);

  assert(alGetError() == AL_NO_ERROR);
  alGenBuffers(static_cast<ALsizei>(_buffers.size()), _buffers.data());
  assert(alGetError() == AL_NO_ERROR);

  if(_envelope)
    _envelope->Press();
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


void Waveform::UpdateBackBuffer(std::vector<int16_t> & data)
{
  if(_envelope)
    for(unsigned int i = 0; i < data.size(); i++)
      data[i] = static_cast<int16_t>(_envelope->GetNextAmplitude() * static_cast<double>(data[i]));

  auto buffer = _buffers[_current_buffer ^ 1];

  assert(alGetError() == AL_NO_ERROR);
  alBufferData(buffer, AL_FORMAT_MONO16, data.data(), static_cast<ALsizei>(data.size() * sizeof(int16_t)), 44100);
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


void Waveform::SetEnvelope(SoundEnvelope * envelope)
{
  _envelope = envelope;
}


SoundEnvelope * Waveform::GetEnvelope() const
{
  return _envelope;
}
