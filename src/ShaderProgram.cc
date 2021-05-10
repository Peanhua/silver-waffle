#include <cassert>
#include "ShaderProgram.hh"
#include "Shader.hh"
#include "Mesh.hh"


ShaderProgram::ShaderProgram(const std::vector<std::string> & vertex_shaders, const std::vector<std::string> & fragment_shaders)
{
  _program = glCreateProgram();
  for(auto src : vertex_shaders)
    {
      auto shader = new Shader(GL_VERTEX_SHADER, src);
      assert(shader != 0);
      glAttachShader(_program, shader->GetShader());
    }

  for(auto src : fragment_shaders)
    {
      auto shader = new Shader(GL_FRAGMENT_SHADER, src);
      assert(shader != 0);
      glAttachShader(_program, shader->GetShader());
    }

  glBindAttribLocation(_program, Mesh::ALOC_VERTEX,   "in_vertex");
  glBindAttribLocation(_program, Mesh::ALOC_COLOR,    "in_color");
  glBindAttribLocation(_program, Mesh::ALOC_TEXCOORD, "in_texcoord");

  glLinkProgram(_program);
}


ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(_program);
}


GLuint ShaderProgram::GetProgram() const
{
  return _program;
}


void ShaderProgram::SetMatrix(const std::string & name, const glm::mat4 & matrix)
{
  auto loc = glGetUniformLocation(_program, name.c_str());
  assert(loc >= 0);
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}


void ShaderProgram::Use() const
{
  assert(_program);
  glUseProgram(_program);
}


