#include "Scene.hh"
#include "ObjectInvader.hh"
#include "ObjectProjectile.hh"
#include "SubsystemAssetLoader.hh"
#include "ObjectSpaceship.hh"
#include "MeshOverlay.hh"
#include <iostream>


static Mesh * overlay = nullptr;

Scene::Scene()
  : _random_generator(0),
    _projectilepos(0)
{
  std::uniform_real_distribution<float> rdist(0, 1);
  for(int i = 0; i < 1000; i++)
    {
      auto b = new ObjectProjectile(this);

      auto rotangle = glm::normalize(glm::vec3(rdist(_random_generator), rdist(_random_generator), rdist(_random_generator)));
      b->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + static_cast<double>(rdist(_random_generator)) * 10.0);

      _projectiles.push_back(b);
    }

  overlay = new MeshOverlay();
  overlay->UpdateGPU();
}


void Scene::Draw(const glm::mat4 & mvp) const
{
  _player->Draw(mvp);

  for(auto i : _invaders)
    i->Draw(mvp);

  for(auto b : _projectiles)
    b->Draw(mvp);

  overlay->Draw(glm::mat4(1));
}


void Scene::Initialize(double difficulty)
{
  assert(difficulty == difficulty);
  
  glm::vec3 topleft(-10, 40, 0);

  _player = new ObjectSpaceship(this);
  _player->SetPosition(topleft + glm::vec3(0, -53, 0));
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
  
  _invaders.clear();
  for(int y = 0; y < 10; y++)
    for(int x = 0; x < 20; x++)
      {
        auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()));
        invader->SetPosition(topleft + glm::vec3(x, y, 0));
        invader->RotateYaw(180.0);

        auto mesh = AssetLoader->LoadMesh("Invader1");
        assert(mesh);
        invader->SetMesh(mesh);

        invader->AddWeapon(glm::vec3(0, -1, 0),
                           AssetLoader->LoadMesh("Projectile"),
                           glm::vec3(0, -1, 0),
                           5.0,
                           34.0);
        
        _invaders.push_back(invader);
      }
}


ObjectSpaceship * Scene::GetPlayer() const
{
  return _player;
}


void Scene::AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime)
{
  bool done = false;
  for(unsigned int i = 0; !done && i < _projectiles.size(); i++)
    {
      if(!_projectiles[_projectilepos]->IsAlive())
        {
          _projectiles[_projectilepos]->Activate(owner, position, velocity, damage, lifetime);
          done = true;
        }

      _projectilepos++;
      if(_projectilepos >= _projectiles.size())
        _projectilepos = 0;
    }
}



void Scene::Tick(double deltatime)
{
  if(_player->IsAlive())
    _player->Tick(deltatime);
  
  for(auto i : _invaders)
    if(i->IsAlive())
      i->Tick(deltatime);
  
  for(auto projectile : _projectiles)
    if(projectile->IsAlive())
      {
        projectile->Tick(deltatime);
        
        ObjectSpaceship * target = nullptr;
        glm::vec3 hitdir;
        
        if(projectile->GetOwner() == _player)
          {
            for(auto i : _invaders)
              if(i->IsAlive())
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
                hitdir.z = 0.0f;
                target = _player;
              }
          }

        if(target)
          {
            target->Hit(projectile->GetDamage(), hitdir);
            projectile->Hit(projectile->GetDamage(), -hitdir);
          }
      }
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

