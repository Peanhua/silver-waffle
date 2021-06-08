#include "Scene.hh"
#include "Camera.hh"
#include "Explosion.hh"
#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "ObjectInvader.hh"
#include "ObjectProjectile.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "Widget.hh"
#include "WormholeWall.hh"
#include <iostream>


Scene::Scene()
  : _random_generator(0),
    _play_area_size(glm::vec2(20, 60)),
    _player(nullptr),
    _on_destroyed_callback(nullptr),
    _on_collectible_collected_callback(nullptr)
{
  std::uniform_real_distribution<float> rdist(0, 1);
  for(int i = 0; i < 300; i++)
    {
      auto b = new ObjectProjectile(this);

      auto rotangle = glm::normalize(glm::vec3(rdist(_random_generator), rdist(_random_generator), rdist(_random_generator)));
      b->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + static_cast<double>(rdist(_random_generator)) * 10.0);

      _projectiles.push_back(b);
    }

  std::minstd_rand random(_random_generator());
  for(int i = 0; i < 100; i++)
    _explosions.push_back(new Explosion(random));

  _wall = new WormholeWall(100, 4);
}


void Scene::Draw(const Camera & camera) const
{
  const glm::mat4 & view       = camera.GetView();
  const glm::mat4 & projection = camera.GetProjection();
  const glm::mat4 & vp         = camera.GetViewProjection();

  glEnable(GL_DEPTH_TEST);

  for(auto p : _planets)
    p->Draw(view, projection, vp);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  if(_player->IsAlive())
    _player->Draw(view, projection, vp);

  for(auto i : _invaders)
    if(i && i->IsAlive())
      i->Draw(view, projection, vp);

  for(auto b : _projectiles)
    if(b->IsAlive())
      b->Draw(view, projection, vp);

  for(auto c : _collectibles)
    if(c && c->IsAlive())
      c->Draw(view, projection, vp);
  
  for(auto e : _explosions)
    if(e->IsAlive())
      e->Draw(view, projection, vp);

  // Two walls (left and right), todo: test doing a half&full circle of walls, maybe left&right normally, and half/full circle during bonus levels?
  const auto max_x = GetPlayAreaSize().x * 0.5f + 0.75f; // 0.75 is approximately half of the player ship width, so that the ship never goes through the wall
  for(int i = 0; i < 360; i += 180)
    {
      glm::mat4 model(1);
      model = glm::rotate(model, glm::radians(static_cast<float>(i)), glm::vec3(0, 1, 0));
      model = glm::translate(model, glm::vec3(-max_x, 40 - 53 - 1, -4/2));
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
      _wall->Draw(model, view, projection, vp * model);
    }
}

void Scene::Initialize(double difficulty)
{
  assert(difficulty == difficulty);

  CreatePlayer();

  for(int i = 0; i < 200; i++)
    _invaders.push_back(nullptr);

  for(int i = 0; i < 50; i++)
    _collectibles.push_back(nullptr);
}


ObjectSpaceship * Scene::CreatePlayer()
{
  delete _player;
  
  _player = new ObjectSpaceship(this);
  _player->SetPosition(glm::vec3(0, 40 - 53, 0));
  _player->SetHorizontalPositionLimit(GetPlayAreaSize().x * 0.5f);
  {
    auto mesh = AssetLoader->LoadMesh("Player");
    assert(mesh);
    _player->SetMesh(mesh);
  }
  _player->AddWeapon();
  _player->AddEngine(glm::vec3(-1, 0, 0), 20.0);
  _player->AddEngine(glm::vec3( 1, 0, 0), 20.0);

  return _player;
}


ObjectSpaceship * Scene::GetPlayer() const
{
  return _player;
}


void Scene::AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime)
{
  auto ind = _projectiles.GetNextFreeIndex();
  if(ind < _projectiles.size())
    _projectiles[ind]->Activate(owner, position, velocity, damage, lifetime);
}


void Scene::Tick(double deltatime)
{
  bool evading = _player->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER)->GetTimer() > 0.0;
  
  if(_player->IsAlive())
    _player->Tick(deltatime);
  
  for(auto i : _invaders)
    if(i && i->IsAlive())
      i->Tick(deltatime);

  for(auto i : _invaders)
    if(i && i->IsAlive())
      {
        glm::vec3 hitdir;
        if(!evading && i->CheckCollision(*_player, hitdir))
          {
            hitdir.y = 0;

            auto vel = i->GetVelocity();
            i->Hit(50, -hitdir);
            if(!i->IsAlive())
              {
                AddExplosion(i->GetPosition(), vel);
                if(_on_destroyed_callback)
                  _on_destroyed_callback(_player, i);
              }

            vel = _player->GetVelocity();
            _player->Hit(50, hitdir);
            if(!_player->IsAlive())
              {
                AddExplosion(_player->GetPosition(), vel);
                if(_on_destroyed_callback)
                  _on_destroyed_callback(i, _player);
              }
            
          }

        if(i->IsAlive())
          {
            const auto max_x = GetPlayAreaSize().x * 0.5f + 0.75f;
            if(std::abs(i->GetPosition().x) > max_x || i->GetPosition().y < 40.0f - 53.0f - 2.0f)
              i->Hit(99999, -glm::normalize(i->GetVelocity()));
          }
      }

  for(auto collectible : _collectibles)
    if(collectible && collectible->IsAlive())
      {
        collectible->Tick(deltatime);

        glm::vec3 hitdir;
        if(!evading && collectible->CheckCollision(*_player, hitdir))
          {
            AddExplosion(collectible->GetPosition(), -hitdir);
            if(_on_collectible_collected_callback)
              _on_collectible_collected_callback(collectible);
            collectible->Hit(99999, -hitdir);
          }
        else
          {
            if(std::abs(collectible->GetPosition().x) > GetPlayAreaSize().x * 0.5f || collectible->GetPosition().y < 40.0f - 53.0f - 2.0f)
              collectible->Hit(99999, -glm::normalize(collectible->GetVelocity()));
          }
      }

  for(auto projectile : _projectiles)
    if(projectile->IsAlive())
      {
        projectile->Tick(deltatime);
        
        ObjectMovable * target = nullptr;
        glm::vec3 hitdir;
        
        if(projectile->GetOwner() == _player)
          {
            for(auto i : _invaders)
              if(i && i->IsAlive())
                if(projectile->CheckCollision(*i, hitdir))
                  {
                    target = i;
                    hitdir = glm::normalize(hitdir + projectile->GetVelocity() + target->GetVelocity());
                    break;
                  }
          }
        else
          {
            if(!evading && projectile->CheckCollision(*_player, hitdir))
              {
                hitdir.y = 0.0f;
                target = _player;
              }
          }

        if(target)
          {
            auto vel = target->GetVelocity();
            target->Hit(projectile->GetDamage(), hitdir);
            projectile->Hit(projectile->GetDamage(), -hitdir);

            if(!target->IsAlive())
              {
                AddExplosion(target->GetPosition(), vel);
                if(_on_destroyed_callback)
                  _on_destroyed_callback(projectile->GetOwner(), target);
              }
          }
        else
          {
            const auto max_x = GetPlayAreaSize().x * 0.5f + 0.75f;
            if(std::abs(projectile->GetPosition().x) > max_x)
              projectile->Hit(99999, -glm::normalize(projectile->GetVelocity()));
          }
      }

  for(auto e : _explosions)
    if(e->IsAlive())
      e->Tick(deltatime);

  _wall->Tick(deltatime);

  if(_player->IsAlive())
    for(auto p : _planets)
      p->Tick(deltatime);
}


void Scene::AddExplosion(const glm::vec3 & position, const glm::vec3 & velocity)
{
  auto ind = _explosions.GetNextFreeIndex();
  if(ind < _explosions.size())
    _explosions[ind]->Activate(position, velocity);
}


void Scene::SetOnDestroyed(on_destroyed_t callback)
{
  assert(!_on_destroyed_callback);
  _on_destroyed_callback = callback;
}


void Scene::SetOnCollectibleCollected(on_collectible_collected_t callback)
{
  assert(!_on_collectible_collected_callback);
  _on_collectible_collected_callback = callback;
}


ObjectInvader * Scene::AddInvader(const glm::vec3 & position)
{
  if(Settings->GetBool("cheat_no_enemies"))
    return nullptr;

  auto ind = _invaders.GetNextFreeIndex();
  if(ind >= _invaders.size())
    return nullptr;
  
  auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()));
  invader->SetPosition(position);
  invader->RotateYaw(180.0);
      
  auto mesh = AssetLoader->LoadMesh("Invader1");
  assert(mesh);
  invader->SetMesh(mesh);
      
  invader->AddWeapon();

  delete _invaders[ind];
  _invaders[ind] = invader;

  return invader;
}


bool Scene::AddCollectible(ObjectCollectible * collectible, const glm::vec3 & position, const glm::vec3 & velocity)
{
  auto ind = _collectibles.GetNextFreeIndex();
  if(ind >= _collectibles.size())
    return false;

  collectible->SetPosition(position);
  collectible->AddImpulse(velocity);
  collectible->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0)), 1.0f);

  delete _collectibles[ind];
  _collectibles[ind] = collectible;

  return true;
}


const glm::vec2 & Scene::GetPlayAreaSize() const
{
  return _play_area_size;
}


void Scene::ClearPlanets()
{
  _planets.clear();
}


void Scene::AddPlanet(Object * planet)
{
  assert(planet);
  _planets.push_back(planet);
}
