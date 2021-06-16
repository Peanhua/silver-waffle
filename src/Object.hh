#ifndef OBJECT_HH_
#define OBJECT_HH_

#include "glm.hh"
#include <algorithm>

class Camera;
class Mesh;
class Scene;


class Object
{
public:
  enum class CollisionChannel
    {
      PLAYER,
      ENEMY,
      PROJECTILE,
      COLLECTIBLE,
    };
  
  //  typedef std::function<void(const Object & other, const glm::vec3 & hit_direction)> on_collision_t;
  typedef std::function<void(Object * destroyer)> on_destroyed_t;
  
  
  Object(Scene * scene);
  virtual ~Object();

  void         Draw(const Camera & camera) const;
  virtual void Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const;
  virtual void Tick(double deltatime);
  virtual void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse);
  virtual void OnDestroyed(Object * destroyer);
  void         SetOnDestroyed(on_destroyed_t callback);

  Mesh * GetMesh() const;
  void   SetMesh(Mesh * mesh);

  bool   CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const;
  virtual void OnCollision(Object & other, const glm::vec3 & hit_direction);
  // void AddOnCollision(on_collision_t callback);

  void AddToCollisionChannel(CollisionChannel channel);
  void AddCollidesWithChannel(CollisionChannel channel);
  
  bool   IsAlive() const;
  double GetHealth() const;
  void   SetHealth(double health);
  double GetMaxHealth() const;
  void   SetMaxHealth(double health);

  void              SetPosition(const glm::vec3 & position);
  void              Translate(const glm::vec3 & translation);
  const glm::vec3 & GetPosition() const;
  void   SetAutoDestroyBox(const glm::vec3 & low, const glm::vec3 & high);

  glm::vec3 GetForwardVector() const;
  glm::vec3 GetRightVector()   const;
  glm::vec3 GetUpVector()      const;
  void RotateRoll(double angle);
  void RotatePitch(double angle);
  void RotateYaw(double angle);
  void Rotate(const glm::quat & rotation);
  const glm::quat & GetOrientation() const;
  void SetOrientation(const glm::quat & orientation);

  void   SetCollisionSphereRadius(double radius);
  double GetCollisionSphereRadius() const;

  Scene * GetScene() const;
  void    SetScene(Scene * scene);


 
private:
  Scene *   _scene;
  glm::vec3 _position;
  glm::quat _orientation;
  Mesh *    _mesh;
  double    _health;
  double    _max_health;
  double    _collision_sphere_radius;
  uint64_t  _collision_channels; // Collision channels this object belongs to.
  uint64_t  _collision_mask;     // Collision channels this object collides with.
  glm::vec3 _destroybox_low;
  glm::vec3 _destroybox_high;
  
  //  std::vector<on_collision_t> _on_collision;
  std::vector<on_destroyed_t> _on_destroyed;
};

#endif
