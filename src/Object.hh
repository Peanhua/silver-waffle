#ifndef OBJECT_HH_
#define OBJECT_HH_

#include "glm.hh"
#include <algorithm>
#include <random>

class Camera;
class CollisionShape;
class Controller;
class Loot;
class Mesh;
class Scene;


class Object
{
public:
  enum class CollisionChannel
    {
      PLAYER,
      ENEMY,
      TERRAIN,
      PROJECTILE,
      COLLECTIBLE,
    };
  
  //  typedef std::function<void(const Object & other, const glm::vec3 & hit_direction)> on_collision_t;
  typedef std::function<void(Object * destroyer)> on_destroyed_t;
  
  
  Object(Scene * scene);
  virtual ~Object();
  Object(const Object & source);
  Object(Object &&)                  = delete; // todo: move constructor
  Object & operator=(const Object &) = delete; // todo: copy assignment
  Object & operator=(Object &&)      = delete; // todo: move assignment

  void         Draw(const Camera & camera) const;
  virtual void Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const;
  virtual void Tick(double deltatime);
  virtual void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse);
  virtual void OnDestroyed(Object * destroyer);
  void         SetOnDestroyed(on_destroyed_t callback);
  void         Destroy(Object * destroyer);
  void         Revive(unsigned int health);

  Mesh * GetMesh() const;
  void   SetMesh(Mesh * mesh);
  virtual double GetVisualBoundingSphereRadius() const;

  CollisionShape * GetCollisionShape() const;
  void             SetCollisionShape(CollisionShape * collision_shape);
  bool         CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const;
  virtual void OnCollision(Object & other, const glm::vec3 & hit_direction);
  // void AddOnCollision(on_collision_t callback);

  void AddToCollisionChannel(CollisionChannel channel);
  void RemoveFromCollisionChannel(CollisionChannel channel);
  void SetCollisionChannels(uint64_t channels);
  void AddCollidesWithChannel(CollisionChannel channel);
  void RemoveCollidesWithChannel(CollisionChannel channel);
  uint64_t GetCollisionChannels() const;
  virtual uint64_t GetCollidesWithChannels() const;

  void   SetUseHealth(bool enable);
  bool   IsAlive() const;
  double GetHealth() const;
  void   SetHealth(double health);
  double GetMaxHealth() const;
  void   SetMaxHealth(double health);

  void              SetPosition(const glm::vec3 & position);
  void              Translate(const glm::vec3 & translation);
  const glm::vec3 & GetPosition() const;
  void   SetAutoDestroyBox(const glm::vec3 & low, const glm::vec3 & high);
  enum class ExceedAction
    {
      IGNORE,
      STOP,
      WRAP,
      DESTROY
    };
  void   SetOnExceedingPlayAreaLimits(unsigned int axis, ExceedAction action);

  glm::vec3 GetForwardVector() const;
  glm::vec3 GetRightVector()   const;
  glm::vec3 GetUpVector()      const;
  void RotateRoll(double angle);
  void RotatePitch(double angle);
  void RotateYaw(double angle);
  void Rotate(const glm::quat & rotation);
  const glm::quat & GetOrientation() const;
  void SetOrientation(const glm::quat & orientation);

  Scene * GetScene() const;
  void    SetScene(Scene * scene);

  Controller * GetController() const;
  void         SetController(Controller * controller);

  bool IsSleeping() const;
  void SetIsSleeping(bool sleeping);

  void ClearLoot();
  void AddLoot(Loot * loot);
 
private:
  std::mt19937_64                       _random_generator;
  std::uniform_real_distribution<float> _rdist;
  Scene *      _scene;
  Controller * _controller;
  CollisionShape * _collision_shape;
  glm::vec3    _position;
  glm::quat    _orientation;
  ExceedAction _exceed_actions[3];
  Mesh *    _mesh;
  bool      _sleeping;
  bool      _destroyed;
  bool      _use_health;
  double    _health;
  double    _max_health;
  uint64_t  _collision_channels; // Collision channels this object belongs to.
  uint64_t  _collision_mask;     // Collision channels this object collides with.
  glm::vec3 _destroybox_low;
  glm::vec3 _destroybox_high;
  std::vector<Loot *> _lootset;
  
  //  std::vector<on_collision_t> _on_collision;
  std::vector<on_destroyed_t> _on_destroyed;

  void SpawnLoot();
};

#endif
