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
#include <vector>

class Shader;


class ShaderProgram
{
public:
  ShaderProgram(const std::string & vertex_shader,
                const std::string & fragment_shader,
                const std::string & geometry_shader);
  ~ShaderProgram();

  void Use() const;
  GLuint GetProgram() const;

  void SetMatrix(const std::string & name, const glm::mat4 & matrix);
  void SetVec(const std::string & name, const glm::vec3 & value);
  void SetVec(const std::string & name, const glm::vec4 & value);
  void SetFloat(const std::string & name, float value);
  void SetInt(const std::string & name, int value);

  static GLuint current_program;
  
private:
  GLuint                _program;
  std::vector<Shader *> _shaders;

#ifndef NDEBUG
public:
  void SetName(const std::string & name);
  const std::string & GetName() const;
private:
  std::string _name;
#endif
};

#endif
