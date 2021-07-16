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
#include "Explosion.hh"
#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "ObjectInvader.hh"
#include "ObjectPlanet.hh"
#include "ObjectProjectile.hh"
#include "ObjectSpaceship.hh"
#include "ShaderProgram.hh"
#include "SpaceParticles.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "Widget.hh"
#include <iostream>


Scene::Scene(const glm::vec3 & play_area_size, const std::array<bool, 3> & play_area_wraps)
  : _gravity(0, 0, 0),
    _particles(nullptr),
    _play_area_wraps(play_area_wraps),
    _random_generator(0),
    _rdist(0, 1),
    _play_area_size(play_area_size),
    _player(nullptr),
    _time(0),
    _warp_engine_starting(false),
    _tutorialmessages_enabled(true)
{
  std::minstd_rand random(_random_generator());
  for(int i = 0; i < 100; i++)
    _explosions.push_back(new Explosion(random));

  CreatePlayer();
}


void Scene::Draw(const Camera & camera) const
{
  {
    std::vector<std::string> shadernames
      {
        "SceneObject-Color",
        "SceneObject-Texture",
        "BonusLevelEntrance",
      };
    for(auto name : shadernames)
      {
        auto shader = AssetLoader->LoadShaderProgram(name);
        assert(shader);
        shader->Use();
        shader->SetVec("in_light_color", glm::vec3(1, 1, 1));
        shader->SetFloat("in_time", static_cast<float>(_time));
      }
    ShaderProgram::SetUBOMatrix("Data", "in_view",       camera.GetView());
    ShaderProgram::SetUBOMatrix("Data", "in_projection", camera.GetProjection());
  }
      
  glEnable(GL_DEPTH_TEST);

  for(auto p : _planets)
    p->Draw(camera);

  glClear(GL_DEPTH_BUFFER_BIT);

  if(_particles)
    _particles->Draw(camera);
  
  std::vector<Object *> objects;
  for(auto o : _objects)
    if(o && o->IsAlive())
      objects.push_back(o);

  if(_player->IsAlive())
    objects.push_back(_player);

  for(auto i : _invaders)
    if(i && i->IsAlive())
      objects.push_back(i);

  for(auto b : _projectiles)
    if(b->IsAlive())
      objects.push_back(b);

  for(auto c : _collectibles)
    if(c && c->IsAlive())
      objects.push_back(c);
  
  for(auto o : objects)
    o->Draw(camera);

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
  _player->AddEngine(glm::vec3(-1, 0, 0), 20.0);
  _player->AddEngine(glm::vec3( 1, 0, 0), 20.0);
  _player->AddEngine(glm::vec3( 0, 0, 0), 20.0);
  _player->AddEngine(glm::vec3( 0, 0, 0), 20.0);
  // Planet engines:
  _player->AddEngine(glm::vec3(0,  0,  1), 20.0);
  _player->AddEngine(glm::vec3(0,  0, -1), 20.0);
  _player->AddEngine(glm::vec3(0, -1,  0), 20.0);
  _player->AddEngine(glm::vec3(0, -1,  0), 20.0);
  
  _player->SystemlogEnable();
  _player->SystemlogAppend("Spaceship computer online.\n");
  
}


ObjectSpaceship * Scene::GetPlayer() const
{
  return _player;
}


void Scene::AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime)
{
  auto ind = _projectiles.GetNextFreeIndex();
  if(ind >= _projectiles.size())
    {
      assert(_projectiles.size() < 300);
      ind = static_cast<unsigned int>(_projectiles.size());
      
      auto b = new ObjectProjectile(this);

      auto rotangle = glm::normalize(glm::vec3(_rdist(_random_generator), _rdist(_random_generator), _rdist(_random_generator)));
      b->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + static_cast<double>(_rdist(_random_generator)) * 10.0);
      SetupSceneObject(b, true);

      _projectiles.push_back(b);
    }
  assert(ind < _projectiles.size());
  if(ind < _projectiles.size())
    _projectiles[ind]->Activate(owner, position, velocity, damage, lifetime);
}


void Scene::Tick(double deltatime)
{
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

  std::vector<Object *> objects;

  if(_player && _player->IsAlive())
    objects.push_back(_player);

  for(auto i : _invaders)
    if(i && i->IsAlive())
      objects.push_back(i);

  for(auto c : _collectibles)
    if(c && c->IsAlive())
      objects.push_back(c);
  
  for(auto p : _projectiles)
    if(p && p->IsAlive())
      objects.push_back(p);

  if(_player && _player->IsAlive())
    {
      for(auto o : _objects)
        if(o && o->IsAlive())
          objects.push_back(o);
      
      for(auto p : _planets)
        if(p && p->IsAlive())
          objects.push_back(p);
    }
  
  _collisioncheck_statistics.elapsed_time += deltatime;
  _collisioncheck_statistics.elapsed_frames++;
  for(auto o : objects)
    if(o->IsAlive())
      {
        auto ClearReferences = [this](Object * obj)
        { // todo: Use smart pointers instead of manually fixing references.
          for(auto proj : _projectiles)
            if(proj && proj->IsAlive())
              if(proj->GetOwner() == obj)
                proj->SetOwner(nullptr);
        };

        if(!o->IsSleeping())
          o->Tick(deltatime);
        if(o != GetPlayer() && warpspeed)
          o->Translate(warpspeedmove);

        if(!warpspeed && o->GetCollidesWithChannels())
          CollisionsForObject(o, objects);

        if(!o->IsAlive())
          ClearReferences(o);
      }

  for(auto e : _explosions)
    if(e && e->IsAlive())
      {
        e->Tick(deltatime);
        if(warpspeed)
          e->Translate(warpspeedmove);
      }

  if(_particles)
    {
      if(_warp_engine_starting)
        _particles->Tick(deltatime * (1.0 + static_cast<double>(5.0f * _warp_throttle)));
      else
        _particles->Tick(deltatime);
    }
}


void Scene::CollisionsForObject(Object * o, std::vector<Object *> & objects)
{
  for(unsigned int i = 0; o->IsAlive() && i < objects.size(); i++)
    {
      auto oo = objects[i];
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
                  if(!oo->IsAlive())
                    {
                      auto ClearReferences = [this](Object * obj)
                      { // todo: Use smart pointers instead of manually fixing references.
                        for(auto proj : _projectiles)
                          if(proj && proj->IsAlive())
                            if(proj->GetOwner() == obj)
                              proj->SetOwner(nullptr);
                      };
                      ClearReferences(oo);
                    }
                }
            }
        }
    }
}

void Scene::AddExplosion(const glm::vec3 & position, const glm::vec3 & velocity)
{
  auto ind = _explosions.GetNextFreeIndex();
  if(ind < _explosions.size())
    _explosions[ind]->Activate(position, velocity);
}


ObjectInvader * Scene::AddInvader(const glm::vec3 & position)
{
  if(Settings->GetBool("cheat_no_enemies"))
    return nullptr;

  auto ind = _invaders.GetNextFreeIndex();
  if(ind >= _invaders.size())
    {
      ind = static_cast<unsigned int>(_invaders.size());
      _invaders.push_back(nullptr);
    }
  
  auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()));
  invader->SetPosition(position);
  invader->RotateYaw(180.0);

  SetupSceneObject(invader, true);
  
  delete _invaders[ind];
  _invaders[ind] = invader;

  return invader;
}


void Scene::AddCollectible(ObjectCollectible * collectible, const glm::vec3 & position)
{
  assert(collectible->IsAlive());
  
  auto ind = _collectibles.GetNextFreeIndex();
  if(ind >= _collectibles.size())
    {
      ind = static_cast<unsigned int>(_collectibles.size());
      _collectibles.push_back(nullptr);
    }

  collectible->SetScene(this);
  collectible->SetPosition(position);
  collectible->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0)), 1.0f);

  SetupSceneObject(collectible, true);
  
  delete _collectibles[ind];
  _collectibles[ind] = collectible;

  TutorialMessage(2, "A valuable item from the explosion,\ncollect it!\n");
}


bool Scene::AddObject(Object * object, const glm::vec3 & position)
{
  assert(object->IsAlive());

  auto ind = _objects.GetNextFreeIndex();
  if(ind < _objects.size())
    {
      delete _objects[ind];
      _objects[ind] = object;
    }
  else
    _objects.push_back(object);

  object->SetScene(this);
  object->SetPosition(position);

  SetupSceneObject(object, true);
  
  return true;
}



const glm::vec3 & Scene::GetPlayAreaSize() const
{
  return _play_area_size;
}


void Scene::ClearPlanets()
{
  _planets.clear();
}


void Scene::AddPlanet(Object * object)
{
  assert(object);
  auto ind = _planets.GetNextFreeIndex();
  if(ind < _planets.size())
    _planets[ind] = object;
  else
    _planets.push_back(object);

  SetupSceneObject(object, true);
}


std::vector<ObjectMovable *> * Scene::GetNearbyObjects(const glm::vec3 & position, float radius)
{
  auto rv = new std::vector<ObjectMovable *>();
  assert(rv);

  if(_player && _player->IsAlive())
    if(glm::distance(position, _player->GetPosition()) < radius)
      rv->push_back(_player);

  for(auto o : _invaders)
    if(o && o->IsAlive())
      if(glm::distance(position, o->GetPosition()) < radius)
        rv->push_back(o);

  for(auto o : _collectibles)
    if(o && o->IsAlive())
      if(glm::distance(position, o->GetPosition()) < radius)
        rv->push_back(o);

  return rv;
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
      for(unsigned int i = 0; i < _player->GetWeaponCount(); i++)
        _player->SetWeaponAutofire(i, false);
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


Object * Scene::GetClosestPlanet(const glm::vec3 & position) const
{
  Object * rv = nullptr;
  float rvdist = 0.0f;

  for(auto planet : _planets)
    {
      auto dist = glm::distance(position, planet->GetPosition());
      if(!rv || dist < rvdist)
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

