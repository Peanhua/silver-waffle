#ifndef SHADERPROGRAM_HH_
#define SHADERPROGRAM_HH_
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
#include <map>
#include <string>
#include <string_view>
#include <vector>

class Shader;


class ShaderProgram
{
public:
  ShaderProgram(const std::string & vertex_shader,
                const std::string & fragment_shader,
                const std::string & geometry_shader);
  ~ShaderProgram();

  void UpdateGPU();
  void Use() const;
  GLuint GetProgram() const;

  inline void SetMatrix(const std::string_view & name, const glm::mat4 & matrix) { SetMatrix(_uniform_locations[GetUniformIndex(name)], matrix); }
  inline void SetVec(const std::string_view & name, const glm::vec3 & value)     { SetVec(   _uniform_locations[GetUniformIndex(name)], value);  }
  inline void SetVec(const std::string_view & name, const glm::vec4 & value)     { SetVec(   _uniform_locations[GetUniformIndex(name)], value);  }
  inline void SetFloat(const std::string_view & name, float value)               { SetFloat( _uniform_locations[GetUniformIndex(name)], value);  }
  inline void SetInt(const std::string_view & name, int value)                   { SetInt(   _uniform_locations[GetUniformIndex(name)], value);  }
  
  static GLuint current_program;
  
private:
  std::string           _vertex_shader;
  std::string           _fragment_shader;
  std::string           _geometry_shader;
  GLuint                _program;
  std::vector<Shader *> _shaders;
  std::vector<GLint>    _uniform_locations;

  constexpr unsigned int GetUniformIndex(const std::string_view & name) const
  { // This list needs to be kept in sync with the one in ShaderProgram::ShaderProgram().
    if(name == "in_time")                 return 1;
    else if(name == "in_velocity")        return 2;
    else if(name == "texture0")           return 3;
    else if(name == "texture1")           return 4;
    else if(name == "in_model")           return 5;
    else if(name == "in_mvp")             return 6;
    else if(name == "in_color")           return 7;
    else if(name == "in_particle_length") return 8;
    else if(name == "in_font_color")      return 9;
    else if(name == "in_font_weight")     return 10;
    else if(name == "in_junktime")        return 11;
    return 0;
  }

  void SetMatrix(GLint position, const glm::mat4 & matrix);
  void SetVec(GLint position, const glm::vec3 & value);
  void SetVec(GLint position, const glm::vec4 & value);
  void SetFloat(GLint position, float value);
  void SetInt(GLint position, int value);

  
#ifndef NDEBUG
public:
  void SetName(const std::string & name);
  const std::string & GetName() const;
private:
  std::string _name;
#endif
};

#endif
