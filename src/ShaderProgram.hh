#ifndef SHADERPROGRAM_HH_
#define SHADERPROGRAM_HH_

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glm.hh"

class ShaderProgram
{
public:
  ShaderProgram(const std::vector<std::string> & vertex_shaders, const std::vector<std::string> & fragment_shaders);
  ~ShaderProgram();

  void Use() const;
  GLuint GetProgram() const;
  /*
  // GLint  GetUniformLocation(const std::string & name) const;
  void   SetUniformValue(const std::string & name, bool value);
  void   SetUniformValue(const std::string & name, uint32_t value);
  void   SetUniformValue(const std::string & name, float value);
  */
  void SetMatrix(const std::string & name, const glm::mat4 & matrix);
  void SetFloat(const std::string & name, float value);
  
private:
  GLuint _program;
};

#endif
