#ifndef SHADERPROGRAM_HH_
#define SHADERPROGRAM_HH_

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glm.hh"

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
  /*
  // GLint  GetUniformLocation(const std::string & name) const;
  void   SetUniformValue(const std::string & name, bool value);
  void   SetUniformValue(const std::string & name, uint32_t value);
  void   SetUniformValue(const std::string & name, float value);
  */
  void SetMatrix(const std::string & name, const glm::mat4 & matrix);
  void SetVec(const std::string & name, const glm::vec3 & value);
  void SetVec(const std::string & name, const glm::vec4 & value);
  void SetFloat(const std::string & name, float value);
  void SetInt(const std::string & name, int value);
  
private:
  GLuint                _program;
  std::vector<Shader *> _shaders;
};

#endif
