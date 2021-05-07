#ifndef OBJECT_HH_
#define OBJECT_HH_

#include "glm.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"

class Scene;

class Object
{
public:
  Object(Scene * scene);
  virtual ~Object();
  
  virtual void Draw(const glm::mat4 & mvp) const;
  virtual void Tick(double deltatime);
  virtual void Hit(double damage, const glm::vec3 & impulse);

  Mesh * GetMesh() const;
  void   SetMesh(Mesh * mesh);

  bool   CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const;
  bool   IsAlive() const;
  void   SetHealth(double health);

  void              SetPosition(const glm::vec3 & position);
  const glm::vec3 & GetPosition() const;

protected:
  Scene *   _scene;
  
private:
  glm::vec3 _position;
  Mesh *    _mesh;
  double    _health;
};

#endif
