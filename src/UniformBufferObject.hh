#ifndef UNIFORM_BUFFER_OBJECT_HH_
#define UNIFORM_BUFFER_OBJECT_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"
#include <GL/glew.h>
#include <array>
#include <string>
#include <string_view>
#include <vector>

class UniformBufferObject
{
public:
  UniformBufferObject(const std::string & name);

  GLuint GetUBO() const;

  inline void SetMatrix(const std::string_view & name, const glm::mat4 & matrix) { SetMatrix(GetPos(name), matrix); }
  inline void SetFloat( const std::string_view & name, const float value)        { SetFloat( GetPos(name), value);  }
  inline void SetVec(   const std::string_view & name, const glm::vec2 & value)  { SetVec(   GetPos(name), value);  }
  inline void SetVec(   const std::string_view & name, const glm::vec3 & value)  { SetVec(   GetPos(name), value);  }
  inline void SetVec(   const std::string_view & name, const glm::vec4 & value)  { SetVec(   GetPos(name), value);  }
  
  void Flush();

  static UniformBufferObject * GetUniformBufferObject();
  
private:
  std::string        _name;
  GLuint             _ubo;
  std::vector<float> _buf;
  unsigned int       _flushmin;
  unsigned int       _flushmax;

  void Set(unsigned int pos, unsigned int bytes, const float * value);

  void SetMatrix(unsigned int pos, const glm::mat4 & matrix);
  void SetFloat(unsigned int pos, const float value);
  void SetVec(unsigned int pos, const glm::vec2 & value);
  void SetVec(unsigned int pos, const glm::vec3 & value);
  void SetVec(unsigned int pos, const glm::vec4 & value);

  constexpr unsigned int GetPos(const std::string_view & name) const
  {
    unsigned int pos = 0;
    
    // todo: Determine the positions from the GLSL code.
    if(name == "in_view")
      return pos;
    pos += sizeof(glm::mat4);

    if(name == "in_projection")
      return pos;
    pos += sizeof(glm::mat4);
    
    if(name == "in_colormod")
      return pos;
    pos += 4 * sizeof(float);
    
    if(name == "in_light_color")
      return pos;
    pos += 4 * sizeof(float);
    
    if(name == "in_glow")
      return pos;
    pos += 4 * sizeof(float);
    
    if(name == "in_resolution")
      return pos;
    pos += 2 * sizeof(float);
    
    if(name == "in_time")
      return pos;
    pos += 2 * sizeof(float);

    return 9999;
  }
};



#endif
