#ifndef SCENE_HH_
#define SCENE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"
#include "RingBuffer.hh"
#include <array>
#include <functional>
#include <random>

class Camera;
class Explosion;
class Mesh;
class Object;
class ObjectBuilding;
class ObjectCollectible;
class ObjectInvader;
class ObjectMovable;
class ObjectProjectile;
class ObjectSpaceship;
class QuadTree;
class SpaceParticles;


class Scene
{
public:


  struct CollisionCheckStatistics
  {
  public:
    CollisionCheckStatistics()
      : elapsed_time(0),
        elapsed_frames(0),
        total(0),
        pass_wide_check(0),
        pass_narrow_check(0)
    {
    }
    
    double   elapsed_time;
    uint64_t elapsed_frames;
    uint64_t total;
    uint64_t pass_wide_check;
    uint64_t pass_narrow_check;
  };
    

  
  Scene(const glm::vec3 & play_area_size, const std::array<bool, 3> & play_area_wraps);
  virtual ~Scene();
  
  virtual void      Draw(const Camera & camera) const;
  virtual void      Tick(double deltatime);

  virtual glm::vec3 GetRandomSpawnPosition() = 0;
  virtual bool      AreInSameCollisionPartition(Object * a, Object * b) const = 0;
  
  const glm::vec3 & GetPlayAreaSize() const;

  void              CreatePlayer();
  virtual void      SetupPlayer();
  ObjectSpaceship * GetPlayer() const;

  void              AddPlanet(Object * planet);
  Object *          GetClosestPlanet(const glm::vec3 & position) const;
  ObjectBuilding *  GetClosestSpaceport(const glm::vec3 & position) const;

  ObjectInvader *    AddInvader(unsigned int type, const glm::vec3 & position);
  ObjectProjectile * AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);
  void               AddExplosion(const glm::vec3 & position, const glm::vec3 & velocity);
  void               AddCollectible(ObjectCollectible * collectible, const glm::vec3 & position);
  void               AddObject(Object * object, const glm::vec3 & position = { 0, 0, 0 });
  void               RemoveObject(Object * object);

  void StartWarpEngine();
  void StopWarpEngine();
  bool IsWarpEngineStarting() const;
  bool CanUseWarpEngine() const;

  void TutorialMessage(unsigned int id, const std::string & message);
  void EnableTutorialMessages(bool enabled);

  double            GetTime() const;
  const glm::vec3 & GetGravity() const;

  const CollisionCheckStatistics & GetCollisionCheckStatistics() const;
  void ResetCollisionCheckStatistics();

  glm::vec3 GetClosestGroundSurface(const glm::vec3 & position) const;

  QuadTree * GetQuadTree() const;

  void DestroyAllEnemies();

  void DumpStats() const;

protected:
  glm::vec3           _gravity;
  SpaceParticles *    _particles;
  std::array<bool, 3> _play_area_wraps;
  std::mt19937_64                _random_generator;
  std::uniform_real_distribution<float> _rdist;
  QuadTree *          _quadtree;
  bool                _can_use_warp_engine;

  virtual void      SetupSceneObject(Object * object, bool destroy_on_block) = 0;
  
private:
  glm::vec3                      _play_area_size;
  ObjectSpaceship *              _player;
  RingBuffer<ObjectProjectile *> _projectiles;
  RingBuffer<Explosion *>        _explosions;
  RingBuffer<Object *>           _planets;
  double                         _time;
  bool                           _warp_engine_starting;
  float                          _warp_throttle;
  bool                           _tutorialmessages_enabled;
  std::vector<bool>              _tutorialmessages;
  CollisionCheckStatistics       _collisioncheck_statistics;
  std::vector<Object *>          _tick_work_objects;
  std::vector<Object *>          _garbage;
  
  void CollisionsForObject(Object * o);
};


#endif
