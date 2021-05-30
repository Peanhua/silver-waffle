#ifndef MESH_PROGRESSBAR_HH_
#define MESH_PROGRESSBAR_HH_

#include "Mesh.hh"

class MeshProgressBar : public Mesh
{
public:
  MeshProgressBar(float width, float height, const glm::vec3 & color);

  void  SetValue(float value);
  float GetValue() const;
  
private:
  float _width;
  float _height;
  float _value;
};

#endif
