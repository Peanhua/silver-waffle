#include "Shader.hh"
#include <cassert>
#include <iostream>

Shader::Shader(GLenum type, const std::string & source)
{
  _shader = glCreateShader(type);

  const char * sources[] = { source.c_str() };
  glShaderSource(_shader, 1, sources, nullptr);
  assert(_shader != 0);
  glCompileShader(_shader);

  GLint compile_success;
  glGetShaderiv(_shader, GL_COMPILE_STATUS, &compile_success);
  if(compile_success == GL_FALSE)
    {
      char buf[4096];
      GLsizei len;

      glGetShaderInfoLog(_shader, (sizeof buf) - 1, &len, buf);
      std::cerr << "Failed to compile shader: " << buf << std::endl;
      std::cerr << "Dumping source:" << std::endl;
      std::cerr << source << std::endl;

      glDeleteShader(_shader);
      _shader = 0;

      assert(false);
    }
}


Shader::~Shader()
{
  glDeleteShader(_shader);
}


GLuint Shader::GetShader() const
{
  return _shader;
}
