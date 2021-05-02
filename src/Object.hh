#ifndef OBJECT_HH_
#define OBJECT_HH_

#include "glm.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"

class Object
{
public:
  Object(const glm::vec3 & position);
  virtual ~Object();
  
  virtual void Draw(const glm::mat4 & mvp) const;
  virtual void Tick(double deltatime);

  void SetMesh(Mesh * mesh);
  void AddChild(Object * child);

  void              SetPosition(const glm::vec3 & position);
  const glm::vec3 & GetPosition() const;
  
private:
  glm::vec3             _position;
  std::vector<Object *> _children;
  Mesh *                _mesh;
};

#endif
