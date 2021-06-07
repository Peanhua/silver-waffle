#ifndef OBJECT_HH_
#define OBJECT_HH_

#include "glm.hh"

class Camera;
class Mesh;
class Scene;


class Object
{
public:
  Object(Scene * scene);
  virtual ~Object();

  void         Draw(const Camera & camera) const;
  virtual void Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const;
  virtual void Tick(double deltatime);
  virtual void Hit(double damage, const glm::vec3 & impulse);

  Mesh * GetMesh() const;
  void   SetMesh(Mesh * mesh);

  bool   CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const;
  bool   IsAlive() const;
  double GetHealth() const;
  void   SetHealth(double health);
  double GetMaxHealth() const;
  void   SetMaxHealth(double health);

  void            SetPosition(const glm::vec3 & position);
  void            Translate(const glm::vec3 & translation);
  const glm::vec3 & GetPosition() const;

  glm::vec3 GetForwardVector() const;
  glm::vec3 GetRightVector()   const;
  glm::vec3 GetUpVector()      const;
  void RotateRoll(double angle);
  void RotatePitch(double angle);
  void RotateYaw(double angle);
  void Rotate(const glm::quat & rotation);
  const glm::quat & GetOrientation() const;

protected:
  Scene *   _scene;
  
private:
  glm::vec3 _position;
  glm::quat _orientation;
  Mesh *    _mesh;
  double    _health;
  double    _max_health;
};

#endif
