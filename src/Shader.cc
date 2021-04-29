#include "Shader.hh"
#include <cassert>
#include <iostream>

Shader::Shader(GLenum type, const std::string & source)
{
  this->_shader = glCreateShader(type);

  const char * sources[] = { source.c_str() };
  glShaderSource(this->_shader, 1, sources, nullptr);
  assert(this->_shader != 0);
  glCompileShader(this->_shader);

  GLint compile_success;
  glGetShaderiv(this->_shader, GL_COMPILE_STATUS, &compile_success);
  if(compile_success == GL_FALSE)
    {
      char buf[4096];
      GLsizei len;

      glGetShaderInfoLog(this->_shader, (sizeof buf) - 1, &len, buf);
      std::cerr << "Failed to compile shader: " << buf << std::endl;
      std::cerr << "Dumping source:" << std::endl;
      std::cerr << source << std::endl;

      glDeleteShader(this->_shader);
      this->_shader = 0;

      assert(false);
    }
}


Shader::~Shader()
{
}


GLuint Shader::GetShader() const
{
  return this->_shader;
}
