#include "Scene.hh"
#include "Explosion.hh"
#include "Mesh.hh"
#include "ObjectInvader.hh"
#include "ObjectProjectile.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "Widget.hh"
#include "WormholeWall.hh"


Scene::Scene()
  : _random_generator(0),
    _on_destroyed_callback(nullptr)
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

  _wall = new WormholeWall(1000, 4);
}


void Scene::Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const
{
  glEnable(GL_DEPTH_TEST);
  if(_player->IsAlive())
    _player->Draw(view, projection, vp);

  for(auto i : _invaders)
    if(i && i->IsAlive())
      i->Draw(view, projection, vp);

  for(auto b : _projectiles)
    if(b->IsAlive())
      b->Draw(view, projection, vp);

  for(auto e : _explosions)
    if(e->IsAlive())
      e->Draw(view, projection, vp);

  // Two walls (left and right), todo: test doing a half&full circle of walls, maybe left&right normally, and half/full circle during bonus levels?
  const auto max_x = 10.0f + 0.75f; // 0.75 is approximately half of the player ship width, so that the ship never goes through the wall
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
  
  glm::vec3 topleft(-10, 40, 0);

  _player = new ObjectSpaceship(this);
  _player->SetPosition(glm::vec3(0, topleft.y - 53, topleft.z));
  {
    auto mesh = AssetLoader->LoadMesh("Player");
    assert(mesh);
    _player->SetMesh(mesh);
  }
  _player->AddWeapon(glm::vec3(0, 1, 0),
                     AssetLoader->LoadMesh("Projectile"),
                     glm::vec3(0, 1, 0),
                     10.0,
                     34.0);
  _player->AddEngine(glm::vec3(-1, 0, 0), 20.0);
  _player->AddEngine(glm::vec3( 1, 0, 0), 20.0);

  for(int i = 0; i < 200; i++)
    _invaders.push_back(nullptr);
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
  if(_player->IsAlive())
    _player->Tick(deltatime);
  
  for(auto i : _invaders)
    if(i && i->IsAlive())
      i->Tick(deltatime);

  bool addmore = false;
  for(auto i : _invaders)
    if(i && i->IsAlive())
      {
        glm::vec3 hitdir;
        if(i->CheckCollision(*_player, hitdir))
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
            const auto max_x = 10.0f + 0.75f;
            if(std::abs(i->GetPosition().x) > max_x || i->GetPosition().y < 40.0f - 53.0f - 2.0f)
              i->Hit(99999, -glm::normalize(i->GetVelocity()));
          }
      }
    else
      addmore = true;

  if(addmore)
    {
      auto ind = _invaders.GetNextFreeIndex();
      if(ind < _invaders.size())
        {
          auto rand = [this]() { return (static_cast<double>(_random_generator()) - static_cast<double>(_random_generator.min())) / static_cast<double>(_random_generator.max()); };
          
          auto r = rand();
          if(r < 5.0 * deltatime)
            {
              const auto max_x = 10.0;
              auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()));
              invader->SetPosition(glm::vec3(-max_x + rand() * max_x * 2.0, 40, 0));
              invader->RotateYaw(180.0);

              auto mesh = AssetLoader->LoadMesh("Invader1");
              assert(mesh);
              invader->SetMesh(mesh);
          
              invader->AddWeapon(glm::vec3(0, -1, 0),
                                 AssetLoader->LoadMesh("Projectile"),
                                 glm::vec3(0, -1, 0),
                                 5.0,
                                 34.0);

              delete _invaders[ind];
              _invaders[ind] = invader;
            }
        }
    }
      
  
  for(auto projectile : _projectiles)
    if(projectile->IsAlive())
      {
        projectile->Tick(deltatime);
        
        ObjectSpaceship * target = nullptr;
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
            if(projectile->CheckCollision(*_player, hitdir))
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
            const auto max_x = 10.0f + 0.75f;
            if(std::abs(projectile->GetPosition().x) > max_x)
              projectile->Hit(99999, -glm::normalize(projectile->GetVelocity()));
          }
      }

  for(auto e : _explosions)
    if(e->IsAlive())
      e->Tick(deltatime);

  _wall->Tick(deltatime);
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


#if 0
void Scene2::Initialize(double difficulty)
{
  assert(difficulty == difficulty);
  #if 0
  glm::vec3 zeropos(0, 0, 4);

  for(int y = 0; y < 8; y++)
    for(int x = 0; x < 12; x++)
      {
        glm::vec3 pos = glm::rotateY(zeropos, static_cast<float>(glm::radians(360.0 / 12.0 * static_cast<double>(x))));
        pos.y = static_cast<float>(y);
        auto invader = new Object(pos);
        if((x + y) % 3 == 0)
          invader->SetMesh(new MeshCube2());
        else
          invader->SetMesh(new MeshCube());
        AddChild(invader);
      }
  #endif
}
#endif

