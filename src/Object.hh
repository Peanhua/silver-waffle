#ifndef OBJECT_HH_
#define OBJECT_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "CollisionShape.hh"
#include "glm.hh"
#include <algorithm>
#include <random>

class Camera;
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
  
  
  Object(Scene * scene, unsigned int random_seed);
  virtual ~Object();
  Object(const Object & source);
  Object(Object &&)                  = delete; // todo: move constructor
  Object & operator=(const Object &) = delete; // todo: copy assignment
  Object & operator=(Object &&)      = delete; // todo: move assignment

  void         Draw(const Camera & camera) const;
  void         Draw(const glm::mat4 & vp) const;
  virtual void Tick(double deltatime);
  virtual void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse);
  virtual void OnDestroyed(Object * destroyer);
  void         SetOnDestroyed(on_destroyed_t callback);
  void         Destroy(Object * destroyer);
  void         Revive(unsigned int health);

  Mesh * GetMesh() const;
  void   SetMesh(Mesh * mesh);
  void   SetColor(const glm::vec3 & color);
  void   SetColor(const glm::vec4 & color);
  virtual double GetGlow() const;
  void           SetGlow(double glow);
  virtual double GetVisualBoundingSphereRadius() const;

  CollisionShape * GetCollisionShape() const;
  void             SetCollisionShape(CollisionShape * collision_shape);
  bool         CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const;
  virtual void OnCollision(Object & other, const glm::vec3 & hit_direction);
  // void AddOnCollision(on_collision_t callback);

  void AddToCollisionChannel(CollisionChannel channel);
  void RemoveFromCollisionChannel(CollisionChannel channel);
  void SetCollisionChannels(uint64_t channels);
  uint64_t GetCollisionChannels() const;

  void AddCollidesWithChannel(CollisionChannel channel);
  void RemoveCollidesWithChannel(CollisionChannel channel);
  void SetCollidesWithChannels(uint64_t channels);
  virtual uint64_t GetCollidesWithChannels() const;

  bool   GetUseHealth() const;
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

  float GetRand();

  bool ShouldTick() const;
  void SetTickingRequiresPlayerAlive(bool alive_required);
  void SetTickingRequiresPlayerVisibility(bool visibility_required);

  void CreateCollisionShape(CollisionShape::Type type);

  bool GetUseGarbageCollection() const;
  void SetUseGarbageCollection(bool enabled);
  
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
  glm::vec4 _color;
  double    _glow;
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
  bool      _ticking_requires_player_alive;
  bool      _ticking_requires_player_visibility;
  bool      _use_garbagecollection;
  
  //  std::vector<on_collision_t> _on_collision;
  std::vector<on_destroyed_t> _on_destroyed;

  void SpawnLoot();
};

#endif
