#ifndef SHADER_HH_
#define SHADER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <GL/glew.h>
#include <string>


class Shader
{
public:
  Shader(GLenum type, const std::string & source);
  ~Shader();

  GLuint GetShader() const;
  /*
  // GLint  GetUniformLocation(const std::string & name) const;
  void   SetUniformValue(const std::string & name, bool value);
  void   SetUniformValue(const std::string & name, uint32_t value);
  void   SetUniformValue(const std::string & name, float value);
  */
private:
  GLuint _shader;
};

#endif
