#ifndef OBJECT_HH_
#define OBJECT_HH_

#include <vector>
#include <GL/glew.h>
#include "glm.hh"
#include "Mesh.hh"
#include "Camera.hh"
#include "ShaderProgram.hh"

class Object
{
public:
  Object(const glm::vec3 & position);
  virtual ~Object();
  
  virtual void Draw(const Camera & camera, ShaderProgram & shader) const;
  virtual void Tick(double deltatime);

  void SetMesh(Mesh * mesh);
  void AddChild(Object * child);

  const glm::vec3 & GetPosition() const;
  
private:
  glm::vec3             _position;
  std::vector<Object *> _children;
  Mesh *                _mesh;
};

#endif
