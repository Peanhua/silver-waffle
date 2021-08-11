/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "UniformBufferObject.hh"


UniformBufferObject::UniformBufferObject(const std::string & name)
  : _name(name),
    _ubo(0),
    _flushmax(0)
{
  glGenBuffers(1, &_ubo);
      
  GLsizeiptr size = 2 * sizeof(glm::mat4) + 3 * 4 * sizeof(float) + 2 * 2 * sizeof(float);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, size);

  assert(static_cast<unsigned int>(size) % sizeof(float) == 0);
  _buf.resize(static_cast<unsigned int>(size) / sizeof(float));

  _flushmin = static_cast<unsigned int>(_buf.size());
  
  assert(_ubo != GL_INVALID_INDEX);
}


GLuint UniformBufferObject::GetUBO() const
{
  return _ubo;
}




void UniformBufferObject::Set(unsigned int pos, unsigned int bytes, const float * value)
{
  assert(bytes % sizeof(float) == 0);

  std::memcpy(_buf.data() + pos / sizeof(float), value, bytes);

  _flushmin = std::min(_flushmin, pos);
  _flushmax = std::max(_flushmax, pos + bytes);
}


void UniformBufferObject::Flush()
{
  assert(_flushmax > 0);
  
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, _flushmin, _flushmax - _flushmin, _buf.data() + _flushmin / sizeof(float));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  _flushmin = static_cast<unsigned int>(_buf.size());
  _flushmax = 0;
}


void UniformBufferObject::SetMatrix(unsigned int pos, const glm::mat4 & matrix)
{
  Set(pos, sizeof(glm::mat4), glm::value_ptr(matrix));
}

void UniformBufferObject::SetFloat(unsigned int pos, const float value)
{
  Set(pos, sizeof value, &value);
}

void UniformBufferObject::SetVec(unsigned int pos, const glm::vec4 & value)
{
  Set(pos, sizeof value, glm::value_ptr(value));
}

void UniformBufferObject::SetVec(unsigned int pos, const glm::vec3 & value)
{
  Set(pos, sizeof value, glm::value_ptr(value));
}

void UniformBufferObject::SetVec(unsigned int pos, const glm::vec2 & value)
{
  Set(pos, sizeof value, glm::value_ptr(value));
}


static UniformBufferObject * ubo = nullptr;
UniformBufferObject * UniformBufferObject::GetUniformBufferObject()
{
  if(!ubo)
    ubo = new UniformBufferObject("Data");
  assert(ubo);
  return ubo;
}

