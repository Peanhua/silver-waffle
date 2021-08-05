#ifndef UNIFORM_BUFFER_OBJECT_HH_
#define UNIFORM_BUFFER_OBJECT_HH_

#include "GLM.hh"
#include <GL/glew.h>
#include <string>

class UniformBufferObject
{
public:
  UniformBufferObject(const std::string & name);

  GLuint GetUBO() const;

  void SetMatrix(const std::string & name, const glm::mat4 & matrix);
  void SetFloat(const std::string & name, const float value);
  void SetVec(const std::string & name, const glm::vec2 & value);
  void SetVec(const std::string & name, const glm::vec3 & value);
  void SetVec(const std::string & name, const glm::vec4 & value);

  static UniformBufferObject * GetUniformBufferObject();
  
private:
  std::string _name;
  GLuint      _ubo;

  unsigned int GetPosition(const std::string & name);

};



#endif
