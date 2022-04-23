/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Scene.hh"
#include "Camera.hh"
#include "CollisionShapeOBB.hh"
#include "CollisionShapeSphere.hh"
#include "Component.hh"
#include "Explosion.hh"
#include "Mesh.hh"
#include "NavigationMap.hh"
#include "ObjectBuilding.hh"
#include "ObjectCollectible.hh"
#include "ObjectInvader.hh"
#include "ObjectPlanet.hh"
#include "ObjectProjectile.hh"
#include "ObjectSpaceship.hh"
#include "QuadTree.hh"
#include "ShaderProgram.hh"
#include "SpaceParticles.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemJobs.hh"
#include "SubsystemSettings.hh"
#include "UniformBufferObject.hh"
#include "Weapon.hh"
#include "Widget.hh"
#include <al.h>
#include <future>
#include <iostream>


Scene::Scene(const glm::vec3 & play_area_size, const std::array<bool, 3> & play_area_wraps)
  : _gravity(0, 0, 0),
    _particles(nullptr),
    _play_area_wraps(play_area_wraps),
    _random_generator(0),
    _rdist(0, 1),
    _quadtree(nullptr),
    _play_area_size(play_area_size),
    _player(nullptr),
    _time(0),
    _warp_engine_starting(false),
    _warp_throttle(0),
    _tutorialmessages_enabled(true),
    _navigation_map(nullptr),
    _tick_finish_sync(0)
{
  auto ratioy = _play_area_size.y / _play_area_size.x;
  auto ratioz = _play_area_size.z / _play_area_size.x;
  _play_area_size.x = std::min(_play_area_size.x, 4000.0f);
  _play_area_size.y = _play_area_size.x * ratioy;
  _play_area_size.z = _play_area_size.x * ratioz;
  
  std::minstd_rand random(_random_generator());
  for(int i = 0; i < 100; i++)
    _explosions.push_back(new Explosion(random));

  _tick_work_objects.reserve(4096);
}


void Scene::Draw(const Camera & camera) const
{
  auto ubo = UniformBufferObject::GetUniformBufferObject();
  ubo->SetMatrix("in_view",        camera.GetView());
  ubo->SetMatrix("in_projection",  camera.GetProjection());
  ubo->SetFloat( "in_time",        static_cast<float>(_time));
  ubo->SetVec(   "in_light_color", glm::vec3(1, 1, 1));
  ubo->Flush();
      
  glEnable(GL_DEPTH_TEST);

  for(auto p : _planets)
    p->Draw(camera);

  glClear(GL_DEPTH_BUFFER_BIT);

  for(auto c : _clouds)
    c->Draw(camera);

  if(_particles)
    _particles->Draw(camera);

  {
    auto & objects = _quadtree->GetNearby2(camera.GetPosition());
    auto o = objects.Next();
    while(o)
      {
        assert(o->IsAlive());
        o->Draw(camera);
        o = objects.Next();
      }
  }

  for(auto e : _explosions)
    if(e->IsAlive())
      e->Draw(camera);
}


Scene::~Scene()
{
}


void Scene::CreatePlayer()
{
  if(_player && _player->IsAlive())
    _player->Destroy(nullptr);
  
  _player = new ObjectSpaceship(this, static_cast<unsigned int>(_random_generator()));
  _player->AddToCollisionChannel(Object::CollisionChannel::PLAYER);
  _player->AddCollidesWithChannel(Object::CollisionChannel::ENEMY);
  _player->SetMesh(AssetLoader->LoadMesh("Player"));
  _player->SetCollisionShape(new CollisionShapeOBB(_player, _player->GetMesh()->GetBoundingBoxHalfSize()));
  _player->AddWeapon();
  
  // Space engines (only 2 first are used):
  _player->AddEngine(glm::vec3(-1,  0, 0), 20.0);
  _player->AddEngine(glm::vec3( 1,  0, 0), 20.0);
  _player->AddEngine(glm::vec3( 0,  1, 0),  5.0);
  _player->AddEngine(glm::vec3( 0, -1, 0),  3.0);
  // Planet engines:
  _player->AddEngine(glm::vec3(0,  0,  1), 20.0);
  _player->AddEngine(glm::vec3(0,  0, -1), 20.0);
  _player->AddEngine(glm::vec3(0, -1,  0), 20.0);
  _player->AddEngine(glm::vec3(0, -1,  0), 20.0);

  _player->SetEngineSound(true);

  auto ee = new ComponentEngineExhaustFX(_player, 2);
  _player->AddComponent(ee);
  ee = new ComponentEngineExhaustFX(_player, 4 + 2);
  _player->AddComponent(ee);
  
  _player->SystemlogEnable();
  _player->SystemlogAppend("Spaceship computer online.\n");
}


ObjectMovable * Scene::GetPlayer() const
{
  return _player;
}


ObjectProjectile * Scene::AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime)
{
  auto ind = _projectiles.GetNextFreeIndex();
  if(ind >= _projectiles.size())
    {
#ifndef NDEBUG
      if(_projectiles.size() > 300)
        std::cout << "Warning, projectiles size has grown large, it is now " << _projectiles.size() << std::endl;
#endif
      ind = static_cast<unsigned int>(_projectiles.size());
      
      auto b = new ObjectProjectile(this);
      b->SetUseGarbageCollection(false);
      auto rotangle = glm::normalize(glm::vec3(_rdist(_random_generator), _rdist(_random_generator), _rdist(_random_generator)));
      b->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + static_cast<double>(_rdist(_random_generator)) * 10.0);
      SetupSceneObject(b, true);

      _projectiles.push_back(b);
    }
  assert(ind < _projectiles.size());
  if(ind < _projectiles.size())
    {
      _projectiles[ind]->Activate(owner, position, velocity, damage, lifetime);
      return _projectiles[ind];
    }
  return nullptr;
}


void Scene::Tick(double deltatime)
{
  if(_player)
    if(Settings->GetBool("sfx"))
      alListenerfv(AL_POSITION, glm::value_ptr(_player->GetPosition()));
  
  if(_warp_engine_starting)
    { // todo: move this warp engine startup code to SpaceshipUpgrade
      // todo:   move _particles to SceneSpace
      if(_player && _player->IsAlive())
        {
          _warp_throttle += static_cast<float>(1.0 / 2.0 * deltatime);
          if(_warp_throttle < 1.0f)
            {
              if(_particles)
                _particles->SetMode(true, _warp_throttle);
            }
          else
            {
              _warp_engine_starting = false;
              if(_particles)
                _particles->SetMode(false);
              auto u = _player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
              u->Activate();
            }
        }
      else
        StopWarpEngine();
    }
  
  bool warpspeed = false;
  glm::vec3 warpspeedmove(0);
  if(_player && _player->IsAlive())
    {
      auto u = _player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
      warpspeed = u->IsActive();
      if(warpspeed)
        warpspeedmove = glm::vec3(0, -1, 0) * static_cast<float>(u->GetValue() * deltatime);
    }
  
  _time += deltatime;
  _collisioncheck_statistics.elapsed_time += deltatime;
  _collisioncheck_statistics.elapsed_frames++;

  auto ProcessExplosivesAndParticles = [this, deltatime, warpspeed, warpspeedmove]()
  {
    for(auto e : _explosions)
      if(e->IsAlive())
        {
          e->Tick(deltatime);
          if(warpspeed)
            e->Translate(warpspeedmove);
        }
    
    if(_particles)
      _particles->Tick(deltatime * (1.0 + 20.0 * static_cast<double>(_warp_throttle)));

    for(auto c : _clouds)
      if(c)
        c->Tick(deltatime);

    _tick_finish_sync.release();

    return false;
  };

  Jobs->AddJob(true, ProcessExplosivesAndParticles);

  
  _tick_work_objects.clear();
  {
    auto all = _quadtree->GetAll();
    auto o = all.Next();
    while(o)
      {
        _tick_work_objects.push_back(o);
        o = all.Next();
      }
  }

  for(auto p : _planets)
    if(p && p->IsAlive())
      _tick_work_objects.push_back(p);

  for(auto o : _tick_work_objects)
    if(o != GetPlayer() && warpspeed)
      o->Translate(warpspeedmove);

  _tick_finish_sync.acquire();

  for(auto o : _tick_work_objects)
    if(o->IsAlive())
      if(o->ShouldTick())
        {
          o->Tick(deltatime);
          
          if(!warpspeed && o->GetCollidesWithChannels())
            CollisionsForObject(o);
        }
}


            
void Scene::CollisionsForObject(Object * o)
{
  auto & objects = _quadtree->GetNearby(o->GetPosition());
  auto oo = objects.Next();
  while(o->IsAlive() && oo)
    {
      if(o != oo && oo->IsAlive())
        {
          _collisioncheck_statistics.total++;
          if(AreInSameCollisionPartition(o, oo))
            {
              _collisioncheck_statistics.pass_wide_check++;
              glm::vec3 hitdir;
              if(o->CheckCollision(*oo, hitdir))
                {
                  _collisioncheck_statistics.pass_narrow_check++;
                  o->OnCollision(*oo, -hitdir);
                }
            }
        }
      oo = objects.Next();
    }
}

void Scene::AddExplosion(const glm::vec3 & position, const glm::vec3 & velocity)
{
  auto ind = _explosions.GetNextFreeIndex();
  if(ind < _explosions.size())
    _explosions[ind]->Activate(position, velocity);
}


ObjectInvader * Scene::AddInvader(unsigned int type, const glm::vec3 & position)
{
  if(Settings->GetBool("cheat_no_enemies"))
    return nullptr;

  auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()), type);
  AddObject(invader, position);

  return invader;
}


void Scene::AddCollectible(ObjectCollectible * collectible, const glm::vec3 & position)
{
  assert(collectible->IsAlive());

  collectible->SetScene(this);
  _quadtree->Add(collectible);
  AddObject(collectible, position);

  TutorialMessage(2, "A valuable item from the explosion,\ncollect it!\n");
}


void Scene::AddObject(Object * object, const glm::vec3 & position)
{
  assert(object->IsAlive());

  object->SetPosition(position);

  SetupSceneObject(object, true);

  assert(object->GetScene() == this);
}



const glm::vec3 & Scene::GetPlayAreaSize() const
{
  return _play_area_size;
}




void Scene::AddPlanet(Object * object)
{
  assert(object);
  _planets.Add(object);
  SetupSceneObject(object, true);
}


void Scene::AddCloud(Object * object)
{
  assert(object);
  _clouds.Add(object);
  SetupSceneObject(object, true);
}


void Scene::StartWarpEngine()
{
  if(!_player || !_player->IsAlive())
    return;
  
  auto u = _player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
  if(u->CanActivate())
    {
      _player->SystemlogAppend("Warp engine: initializing\n");
      _warp_engine_starting = true;
      _warp_throttle = 0;

      for(unsigned int i = 0; i < _player->GetEngineCount(); i++)
        _player->SetEngineThrottle(i, 0.0);
      for(unsigned int group = 0; group < _player->GetWeaponGroupCount(); group++)
        for(auto weapon : _player->GetWeapons(group))
          weapon->SetAutofire(false);
    }
  else if(u->GetInstallCount() > 0)
    _player->SystemlogAppend("Warp engine: error\n");
}

void Scene::StopWarpEngine()
{
  _warp_engine_starting = false;
  _warp_throttle = 0;
  _particles->SetMode(false);

  if(!_player || !_player->IsAlive())
    return;

  auto u = _player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
  u->Deactivate();
}


bool Scene::IsWarpEngineStarting() const
{
  return _warp_engine_starting;
}




void Scene::TutorialMessage(unsigned int id, const std::string & message)
{
  if(!_tutorialmessages_enabled)
    return;
  
  if(!Settings->GetBool("tutorial"))
    return;
  
  if(!_player || !_player->IsAlive())
    return;

  assert(id < 100);
  while(id >= _tutorialmessages.size())
    _tutorialmessages.push_back(false);
  
  if(!_tutorialmessages[id])
    {
      _tutorialmessages[id] = true;
      _player->SystemlogAppend(message);
    }
}


void Scene::EnableTutorialMessages(bool enabled)
{
  _tutorialmessages_enabled = enabled;
}


double Scene::GetTime() const
{
  return _time;
}


Object * Scene::GetNextClosestPlanet(const glm::vec3 & position) const
{
  Object * rv = nullptr;
  float rvdist = 0.0f;

  for(auto planet : _planets)
    if(planet && planet->IsAlive())
      {
        auto dist = planet->GetPosition().y - position.y;
        if(!rv || (dist > 0 && dist < rvdist))
          {
            rv = planet;
            rvdist = dist;
          }
      }
  
  return rv;
}


void Scene::SetupPlayer()
{
  SetupSceneObject(_player, false);
}


const glm::vec3 & Scene::GetGravity() const
{
  return _gravity;
}


const Scene::CollisionCheckStatistics & Scene::GetCollisionCheckStatistics() const
{
  return _collisioncheck_statistics;
}


void Scene::ResetCollisionCheckStatistics()
{
  CollisionCheckStatistics z;
  _collisioncheck_statistics = z;
}


glm::vec3 Scene::GetClosestGroundSurface(const glm::vec3 & position) const
{
  auto pos = position;
  auto & objs = _quadtree->GetNearby(position);
  auto radius = GetPlayAreaSize().z;
  bool retry = true;
  while(retry)
    {
      retry = false;
      objs.Rewind();
      auto o = objs.Next();
      while(o)
        {
          auto op = o->GetPosition();
          if(glm::distance(position, op) < radius)
            if(o->GetCollisionShape())
              {
                auto dims = o->GetMesh()->GetBoundingBoxHalfSize();
                auto topleft = op + glm::vec3(-dims.x, 0, dims.z);
                auto botright = op + glm::vec3(dims.x, 0, -dims.z);
                if(pos.x >= topleft.x  && pos.x <= botright.x &&
                   pos.z >= botright.z && pos.z <= topleft.z    )
                  {
                    retry = true;
                    pos.z = topleft.z + 0.001f;
                  }
              }
          o = objs.Next();
        }
    }

  return pos;
}


QuadTree * Scene::GetQuadTree() const
{
  return _quadtree;
}


void Scene::RemoveObject(Object * object)
{
  _quadtree->Remove(object);
  object->SetScene(nullptr);
}


ObjectBuilding * Scene::GetClosestSpaceport(const glm::vec3 & position) const
{
  auto & objs = _quadtree->GetNearby(position); // todo: Add distance parameter. Because currently, from this point of view, we don't know what the "nearby" means.

  auto o = objs.Next();
  while(o)
    {
      auto spaceport = dynamic_cast<ObjectBuilding *>(o);

      if(spaceport && spaceport->GetIsSpaceport())
        return spaceport;

      o = objs.Next();
    }
  
  return nullptr;
}



bool Scene::CanUseWarpEngine() const
{
  return _can_use_warp_engine;
}


void Scene::DumpStats() const
{
  std::cout << "scene: " << this
            << " play_area_size=" << _play_area_size
            << " projectiles.size=" << _projectiles.size()
            << " explosions.size=" << _explosions.size()
            << " planets.size=" << _planets.size()
            << " time=" << _time
            << std::endl;
#if 0
  auto all = _quadtree->GetAll();
  auto o = all.Next();
  while(o)
    {
      o->GetMesh()->Dump();
      o = all.Next();
    }
  std::cout << std::flush;
#endif
}


void Scene::DestroyAllEnemies()
{
  auto & objs = _quadtree->GetNearby2(_player->GetPosition());
  while(auto o = objs.Next())
    if(o->IsAlive())
      {
        if(dynamic_cast<ObjectInvader *>(o))
          o->Destroy(nullptr);
        else if(dynamic_cast<ObjectProjectile *>(o))
          o->Destroy(nullptr);
      }
}


void Scene::SetNavigationMap(NavigationMap * navigation_map)
{
  _navigation_map = navigation_map;
}


NavigationMap * Scene::GetNavigationMap() const
{
  return _navigation_map;
}

