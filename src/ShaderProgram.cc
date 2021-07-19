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


std::map<std::string, GLuint> ShaderProgram::_ubos;


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


void ShaderProgram::SetMatrix(const std::string & name, const glm::mat4 & matrix)
{
  auto loc = glGetUniformLocation(_program, name.c_str());
  //assert(loc >= 0);
  if(loc >= 0)
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}


void ShaderProgram::SetVec(const std::string & name, const glm::vec3 & value)
{
  auto loc = glGetUniformLocation(_program, name.c_str());
  //assert(loc >= 0);
  if(loc >= 0)
    glUniform3fv(loc, 1, glm::value_ptr(value));
}


void ShaderProgram::SetVec(const std::string & name, const glm::vec4 & value)
{
  auto loc = glGetUniformLocation(_program, name.c_str());
  //assert(loc >= 0);
  if(loc >= 0)
    glUniform4fv(loc, 1, glm::value_ptr(value));
}


void ShaderProgram::SetFloat(const std::string & name, float value)
{
  auto loc = glGetUniformLocation(_program, name.c_str());
  //assert(loc >= 0);
  if(loc >= 0)
    glUniform1f(loc, value);
}


void ShaderProgram::SetInt(const std::string & name, int value)
{
  auto loc = glGetUniformLocation(_program, name.c_str());
  //assert(loc >= 0);
  if(loc >= 0)
    glUniform1i(loc, value);
}


void ShaderProgram::Use() const
{
  assert(_program);
  glUseProgram(_program);
}


void ShaderProgram::SetUBOMatrix(const std::string & ubo_name, const std::string & name, const glm::mat4 & matrix)
{
  unsigned int pos; // todo: find the position properly
  if(name == "in_view")
    pos = 0;
  else
    pos = sizeof(glm::mat4);

  GLuint ubo = GL_INVALID_INDEX;
  auto it = _ubos.find(ubo_name);
  if(it != _ubos.end())
    ubo = (*it).second;
  else
    {
      glGenBuffers(1, &ubo);
      _ubos[ubo_name] = ubo;

      auto size = 2 * sizeof(glm::mat4) + 4 * sizeof(float) + 2 * sizeof(float) + sizeof(float);
      glBindBuffer(GL_UNIFORM_BUFFER, ubo);
      glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
      glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, size);
    }
  assert(ubo != GL_INVALID_INDEX);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof(glm::mat4), glm::value_ptr(matrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderProgram::SetUBOFloat(const std::string & ubo_name, const std::string & name, const float value)
{
  unsigned int pos = 2 * sizeof(glm::mat4) + 4 * sizeof(float) + 2 * sizeof(float); // todo: find the position properly
  assert(name == "in_time");
  GLuint ubo = GL_INVALID_INDEX;
  auto it = _ubos.find(ubo_name);
  if(it != _ubos.end())
    ubo = (*it).second;
  else
    assert(false); // todo

  assert(ubo != GL_INVALID_INDEX);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, &value);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderProgram::SetUBOVec3(const std::string & ubo_name, const std::string & name, const glm::vec3 & value)
{
  unsigned int pos = 2 * sizeof(glm::mat4); // todo: find the position properly
  assert(name == "in_light_color");
  GLuint ubo = GL_INVALID_INDEX;
  auto it = _ubos.find(ubo_name);
  if(it != _ubos.end())
    ubo = (*it).second;
  else
    assert(false); // todo

  assert(ubo != GL_INVALID_INDEX);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, glm::value_ptr(value));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderProgram::SetUBOVec2(const std::string & ubo_name, const std::string & name, const glm::vec2 & value)
{
  unsigned int pos = 2 * sizeof(glm::mat4) + 4 * sizeof(float); // todo: find the position properly
  assert(name == "in_resolution");
  GLuint ubo = GL_INVALID_INDEX;
  auto it = _ubos.find(ubo_name);
  if(it != _ubos.end())
    ubo = (*it).second;
  else
    assert(false); // todo

  assert(ubo != GL_INVALID_INDEX);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, glm::value_ptr(value));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

