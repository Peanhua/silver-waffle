#ifndef MESH_PROGRESSBAR_HH_
#define MESH_PROGRESSBAR_HH_

#include "Mesh.hh"

class MeshProgressBar : public Mesh
{
public:
  MeshProgressBar(float width, float height, const glm::vec3 & color);

  void  SetColor(const glm::vec3 & color);
  void  SetValue(float value);
  float GetValue() const;

  void  PreDrawSetupShader(ShaderProgram * shader_program) const override;
  
private:
  float     _width;
  float     _height;
  glm::vec3 _color;
  float     _value;
};

#endif
