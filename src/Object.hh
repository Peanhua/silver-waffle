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

  void            SetPosition(const glm::vec3 & position);
  void            Translate(const glm::vec3 & translation);
  const glm::vec3 GetPosition() const;

  glm::vec3 GetForwardVector() const;
  glm::vec3 GetRightVector()   const;
  glm::vec3 GetUpVector()      const;
  void RotateRoll(double angle);
  void RotatePitch(double angle);
  void RotateYaw(double angle);

protected:
  Scene *   _scene;
  
private:
  glm::vec3 _position;
  glm::mat4 _orientation;
  Mesh *    _mesh;
  double    _health;
};

#endif
