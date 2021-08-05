#include "UniformBufferObject.hh"


UniformBufferObject::UniformBufferObject(const std::string & name)
  : _name(name),
    _ubo(0)
{
  glGenBuffers(1, &_ubo);
      
  GLsizeiptr size = 2 * sizeof(glm::mat4) + 3 * 4 * sizeof(float) + 2 * 2 * sizeof(float);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, size);
  
  assert(_ubo != GL_INVALID_INDEX);
}


GLuint UniformBufferObject::GetUBO() const
{
  return _ubo;
}



unsigned int UniformBufferObject::GetPosition(const std::string & name)
{
  unsigned int pos = 0;

  // todo: Determine the positions from the GLSL code.
  if(name == "in_view")
    return pos;
  pos += sizeof(glm::mat4);
  
  if(name == "in_projection")
    return pos;
  pos += sizeof(glm::mat4);

  if(name == "in_colormod")
    return pos;
  pos += 4 * sizeof(float);
  
  if(name == "in_light_color")
    return pos;
  pos += 4 * sizeof(float);

  if(name == "in_glow")
    return pos;
  pos += 4 * sizeof(float);

  if(name == "in_resolution")
    return pos;
  pos += 2 * sizeof(float);

  if(name == "in_time")
    return pos;
  pos += 2 * sizeof(float);

  return 9999;
}


void UniformBufferObject::SetMatrix(const std::string & name, const glm::mat4 & matrix)
{
  auto pos = GetPosition(name);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof(glm::mat4), glm::value_ptr(matrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetFloat(const std::string & name, const float value)
{
  auto pos = GetPosition(name);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, &value);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetVec(const std::string & name, const glm::vec4 & value)
{
  auto pos = GetPosition(name);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, glm::value_ptr(value));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetVec(const std::string & name, const glm::vec3 & value)
{
  auto pos = GetPosition(name);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, glm::value_ptr(value));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetVec(const std::string & name, const glm::vec2 & value)
{
  auto pos = GetPosition(name);
  glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, pos, sizeof value, glm::value_ptr(value));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


static UniformBufferObject * ubo = nullptr;
UniformBufferObject * UniformBufferObject::GetUniformBufferObject()
{
  if(!ubo)
    ubo = new UniformBufferObject("Data");
  assert(ubo);
  return ubo;
}

