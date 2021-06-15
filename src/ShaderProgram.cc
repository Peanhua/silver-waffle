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
  }

  {
    auto shader = new Shader(GL_FRAGMENT_SHADER, fragment_shader);
    assert(shader != 0);
    glAttachShader(_program, shader->GetShader());
  }

  if(geometry_shader.size() > 0)
    {
      auto shader = new Shader(GL_GEOMETRY_SHADER, geometry_shader);
      assert(shader != 0);
      glAttachShader(_program, shader->GetShader());
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
