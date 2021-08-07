/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ShaderProgram.hh"
#include "Shader.hh"
#include "Mesh.hh"
#include <cassert>
#include <iostream>


ShaderProgram::ShaderProgram(const std::string & vertex_shader, const std::string & fragment_shader, const std::string & geometry_shader)
{
  _program = glCreateProgram();
  {
    auto shader = new Shader(GL_VERTEX_SHADER, vertex_shader);
    assert(shader != 0);
    glAttachShader(_program, shader->GetShader());
    _shaders.push_back(shader);
  }

  {
    auto shader = new Shader(GL_FRAGMENT_SHADER, fragment_shader);
    assert(shader != 0);
    glAttachShader(_program, shader->GetShader());
    _shaders.push_back(shader);
  }

  if(geometry_shader.size() > 0)
    {
      auto shader = new Shader(GL_GEOMETRY_SHADER, geometry_shader);
      assert(shader != 0);
      glAttachShader(_program, shader->GetShader());
      _shaders.push_back(shader);
    }

  glBindAttribLocation(_program, Mesh::ALOC_VERTEX,       "in_vertex");
  glBindAttribLocation(_program, Mesh::ALOC_COLOR,        "in_color");
  glBindAttribLocation(_program, Mesh::ALOC_TEXCOORD,     "in_texcoord");
  glBindAttribLocation(_program, Mesh::ALOC_NORMAL,       "in_normal");
  glBindAttribLocation(_program, Mesh::ALOC_GENERIC_VEC2, "in_generic_vec2");
  glBindAttribLocation(_program, Mesh::ALOC_GENERIC_VEC3, "in_generic_vec3");
  glBindAttribLocation(_program, Mesh::ALOC_EMISSION,     "in_emission");

  glLinkProgram(_program);

  GLint status;
  glGetProgramiv(_program, GL_LINK_STATUS, &status);
  if(status == GL_FALSE)
    {
      char buf[4096];
      GLsizei len;

      glGetProgramInfoLog(this->_program, (sizeof buf) - 1, &len, buf);
      std::cerr << "Failed to link shader program: " << buf << "\n";
      std::string delimiter = "###############################################################################\n";

      std::cerr << delimiter;
      std::cerr << "# Dumping vertex shader:\n";
      std::cerr << delimiter;
      std::cerr << vertex_shader << "\n";
      std::cerr << delimiter;
      std::cerr << "# End of Vertex shader.\n";
      std::cerr << delimiter << "\n";
      
      std::cerr << delimiter;
      std::cerr << "# Dumping Fragment shader:\n";
      std::cerr << delimiter;
      std::cerr << fragment_shader << "\n";
      std::cerr << delimiter;
      std::cerr << "# End of Fragment shader.\n";
      std::cerr << delimiter << "\n";
      
      std::cerr << delimiter;
      std::cerr << "# Dumping Geometry shader:\n";
      std::cerr << delimiter;
      std::cerr << geometry_shader << "\n";
      std::cerr << delimiter;
      std::cerr << "# End of Geometry shader.\n";
      std::cerr << delimiter;
      
      std::cerr << std::flush;
    }      
  assert(status == GL_TRUE);

  auto ubi = glGetUniformBlockIndex(_program, "Data");
  if(ubi != GL_INVALID_INDEX)
    glUniformBlockBinding(_program, ubi, 0);

  std::vector<std::string> uniforms {
    "in_time",
    "in_velocity",
    "texture0",
    "texture1",
    "in_model",
    "in_mvp",
    "in_color",
    "in_particle_length",
    "in_font_color",
    "in_font_weight",
    "in_junktime"
  };
  _uniform_locations.push_back(-1);
  for(auto u : uniforms)
    _uniform_locations.push_back(glGetUniformLocation(_program, u.c_str()));
}


ShaderProgram::~ShaderProgram()
{
  for(auto shader : _shaders)
    {
      glDetachShader(_program, shader->GetShader());
      delete shader;
    }
  glDeleteProgram(_program);
}


GLuint ShaderProgram::GetProgram() const
{
  return _program;
}


void ShaderProgram::SetMatrix(GLint position, const glm::mat4 & matrix)
{
  if(position >= 0)
    glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(matrix));
}


void ShaderProgram::SetVec(GLint position, const glm::vec3 & value)
{
  if(position >= 0)
    glUniform3fv(position, 1, glm::value_ptr(value));
}


void ShaderProgram::SetVec(GLint position, const glm::vec4 & value)
{
  if(position >= 0)
    glUniform4fv(position, 1, glm::value_ptr(value));
}


void ShaderProgram::SetFloat(GLint position, float value)
{
  if(position >= 0)
    glUniform1f(position, value);
}


void ShaderProgram::SetInt(GLint position, int value)
{
  if(position >= 0)
    glUniform1i(position, value);
}




GLuint ShaderProgram::current_program = 0;

void ShaderProgram::Use() const
{
  assert(_program);

  if(_program == current_program)
    return;

  current_program = _program;
  glUseProgram(_program);
}


#ifndef NDEBUG
void ShaderProgram::SetName(const std::string & name)
{
  _name = name;
}


const std::string & ShaderProgram::GetName() const
{
  return _name;
}
#endif
