#include <cassert>
#include "ShaderProgram.hh"
#include "Shader.hh"


ShaderProgram::ShaderProgram(const std::vector<std::string> & vertex_shaders, const std::vector<std::string> & fragment_shaders)
{
  this->_program = glCreateProgram();
  for(auto src : vertex_shaders)
    {
      auto shader = new Shader(GL_VERTEX_SHADER, src);
      assert(shader != 0);
      glAttachShader(this->_program, shader->GetShader());
    }

  for(auto src : fragment_shaders)
    {
      auto shader = new Shader(GL_FRAGMENT_SHADER, src);
      assert(shader != 0);
      glAttachShader(this->_program, shader->GetShader());
    }

  glBindAttribLocation(this->_program, 0, "in_vertex");
  glBindAttribLocation(this->_program, 1, "in_color");

  glLinkProgram(this->_program);
}

GLuint ShaderProgram::GetProgram() const
{
  return this->_program;
}


void ShaderProgram::SetMatrix(const std::string & name, const glm::mat4 & matrix)
{
  auto loc = glGetUniformLocation(this->_program, name.c_str());
  assert(loc >= 0);
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
