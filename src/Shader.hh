#ifndef SHADER_HH_
#define SHADER_HH_

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
