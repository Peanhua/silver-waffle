#include "Scene.hh"
#include "Camera.hh"
#include "Explosion.hh"
#include "Mesh.hh"
#include "Milkyway.hh"
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
#include "WormholeWall.hh"
#include <iostream>


Scene::Scene()
  : _random_generator(0),
    _play_area_size(glm::vec2(20, 60)),
    _player(nullptr),
    _time(0)
{
  std::uniform_real_distribution<float> rdist(0, 1);
  for(int i = 0; i < 300; i++)
    {
      auto b = new ObjectProjectile(this);

      auto rotangle = glm::normalize(glm::vec3(rdist(_random_generator), rdist(_random_generator), rdist(_random_generator)));
      b->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + static_cast<double>(rdist(_random_generator)) * 10.0);
      {
        const auto m = GetPlayAreaSize();
        b->SetAutoDestroyBox(glm::vec3(-m.x * 0.5f, 40 - 53 - 2, 1),
                             glm::vec3( m.x * 0.5f, m.y,         0));
      }

      _projectiles.push_back(b);
    }

  std::minstd_rand random(_random_generator());
  for(int i = 0; i < 100; i++)
    _explosions.push_back(new Explosion(random));

  _milkyway = new Milkyway();

  _particles = new SpaceParticles(5.0, 50.0, 0);

  _wall = new WormholeWall(100, 4);

  CreatePlayer();

  for(int i = 0; i < 200; i++)
    _invaders.push_back(nullptr);

  for(int i = 0; i < 50; i++)
    _collectibles.push_back(nullptr);

  _time = 0;
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
  }
      
  const glm::mat4 & view       = camera.GetView();
  const glm::mat4 & projection = camera.GetProjection();
  const glm::mat4 & vp         = camera.GetViewProjection();

  glDisable(GL_DEPTH_TEST);
  _milkyway->Draw(camera);
  glEnable(GL_DEPTH_TEST);

  for(auto p : _planets)
    p->Draw(view, projection, vp);
  glClear(GL_DEPTH_BUFFER_BIT);

  for(auto o : _objects)
    if(o && o->IsAlive())
      o->Draw(view, projection, vp);

  _particles->Draw(camera);
  
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


ObjectSpaceship * Scene::CreatePlayer()
{
  if(_player && _player->IsAlive())
    _player->Destroy(nullptr);
  
  _player = new ObjectSpaceship(this);
  _player->AddToCollisionChannel(Object::CollisionChannel::PLAYER);
  _player->AddCollidesWithChannel(Object::CollisionChannel::ENEMY);
  _player->AddCollidesWithChannel(Object::CollisionChannel::PROJECTILE);
  _player->AddCollidesWithChannel(Object::CollisionChannel::COLLECTIBLE);
  _player->EnableVelocity(true, false, false);
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
  assert(ind < _projectiles.size());
  if(ind < _projectiles.size())
    _projectiles[ind]->Activate(owner, position, velocity, damage, lifetime);
}


void Scene::Tick(double deltatime)
{
  bool warpspeed = false;
  double warpspeedmult = 0.0;
  if(_player && _player->IsAlive())
    {
      auto u = _player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
      warpspeed = u->IsActive();
      warpspeedmult = u->GetValue();
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

  double dtime = deltatime;
  if(warpspeed)
    dtime *= warpspeedmult;
  
  for(auto o : objects)
    {
      if(o->IsAlive())
        o->Tick(dtime); // todo: Add separate method to move the objects when warpspeed is active.

      if(!warpspeed)
        for(unsigned int i = 0; o->IsAlive() && i < objects.size(); i++)
          {
            auto oo = objects[i];
            if(o != oo && oo->IsAlive())
              {
                glm::vec3 hitdir;
                if(o->CheckCollision(*oo, hitdir))
                  {
                    o->OnCollision(*oo, -hitdir);
                    oo->OnCollision(*o, hitdir);
                  }
              }
          }
    }
  

  for(auto e : _explosions)
    if(e && e->IsAlive())
      e->Tick(dtime); // todo: Add separate method to move the explosions when warpseed is active.

  _particles->Tick(dtime);
  _wall->Tick(deltatime);
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
  assert(ind < _invaders.size());
  if(ind >= _invaders.size())
    return nullptr;
  
  auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()));
  invader->SetPosition(position);
  invader->RotateYaw(180.0);
  
  {
    const auto m = GetPlayAreaSize();
    invader->SetAutoDestroyBox(glm::vec3(-m.x * 0.5f, 40 - 53 - 2, 1),
                               glm::vec3( m.x * 0.5f, m.y,         0));
  }
  
  delete _invaders[ind];
  _invaders[ind] = invader;

  return invader;
}


bool Scene::AddCollectible(ObjectCollectible * collectible, const glm::vec3 & position, const glm::vec3 & velocity)
{
  assert(collectible->IsAlive());
  
  auto ind = _collectibles.GetNextFreeIndex();
  if(ind >= _collectibles.size())
    return false;

  collectible->SetScene(this);
  collectible->SetPosition(position);
  collectible->SetVelocity(velocity);
  collectible->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0)), 1.0f);

  {
    const auto m = GetPlayAreaSize();
    collectible->SetAutoDestroyBox(glm::vec3(-m.x * 0.5f, 40 - 53 - 2, 1),
                                   glm::vec3( m.x * 0.5f, m.y,         0));
  }
  
  delete _collectibles[ind];
  _collectibles[ind] = collectible;

  return true;
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

  {
    const auto m = GetPlayAreaSize();
    object->SetAutoDestroyBox(glm::vec3(-m.x * 0.5f, 40 - 53 - 2 - object->GetMesh()->GetBoundingSphereRadius(), 1),
                              glm::vec3( m.x * 0.5f,                                                    9999999, 0));
  }
  
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


void Scene::AddPlanet(Object * object)
{
  assert(object);
  auto ind = _planets.GetNextFreeIndex();
  if(ind < _planets.size())
    _planets[ind] = object;
  else
    _planets.push_back(object);

  auto bsr = object->GetMesh()->GetBoundingSphereRadius();

  auto planet = dynamic_cast<ObjectPlanet *>(object);
  if(planet)
    bsr *= 2.0; // todo: query the exact ring size

  const auto m = GetPlayAreaSize();
  planet->SetAutoDestroyBox(glm::vec3(-m.x * 0.5f, 40 - 53 - 2 - bsr, 1),
                            glm::vec3( m.x * 0.5f,           9999999, 0));
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
