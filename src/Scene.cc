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
    _projectilepos(0),
    _explosionpos(0),
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

  _walls.push_back(new WormholeWall(1000, 4));
  _walls.push_back(new WormholeWall(1000, 4));
}


void Scene::Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const
{
  glEnable(GL_DEPTH_TEST);
  if(_player->IsAlive())
    _player->Draw(view, projection, vp);

  for(auto i : _invaders)
    if(i->IsAlive())
      i->Draw(view, projection, vp);

  for(auto b : _projectiles)
    if(b->IsAlive())
      b->Draw(view, projection, vp);

  for(auto e : _explosions)
    if(e->IsAlive())
      e->Draw(view, projection, vp);

  // Two walls (left and right), todo: test doing a half&full circle of walls, maybe left&right normally, and half/full circle during bonus levels?
  const auto max_x = 10.0f + 0.75f; // 0.75 is approximately half of the player ship width, so that the ship never goes through the wall
  const auto wormhole_length = 1000.0f;
  std::vector<glm::vec3> wall_translations {
    glm::vec3( max_x, 40 - 53 - 1 + wormhole_length, -4/2),
    glm::vec3(-max_x, 40 - 53 - 1,                   -4/2),
  };

  std::vector<glm::vec4> wall_rotations {
    glm::vec4(-90, 0, 0, 1),
    glm::vec4( 90, 0, 0, 1),
  };
  
  for(unsigned int i = 0; i < _walls.size(); i++)
    {
      assert(i < wall_translations.size());
      assert(i < wall_rotations.size());
      auto w = _walls[i];
      glm::mat4 model(1);
      model = glm::translate(model, wall_translations[i]);
      model = glm::rotate(model, glm::radians(wall_rotations[i].x), wall_rotations[i].yzw());
      w->Draw(model, view, projection, vp * model);
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
  
  _invaders.clear();
  for(int y = 0; y < 10; y++)
    for(int x = 0; x < 20; x++)
      {
        auto invader = new ObjectInvader(this, static_cast<unsigned int>(_random_generator()));
        invader->SetPosition(topleft + glm::vec3(0.5 + x, y, 0));
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
  
  for(auto i : _invaders)
    if(i->IsAlive())
      {
        glm::vec3 hitdir;
        if(i->CheckCollision(*_player, hitdir))
          {
            hitdir.y = 0;
            
            i->Hit(50, -hitdir);
            if(!i->IsAlive())
              {
                AddExplosion(i->GetPosition());
                if(_on_destroyed_callback)
                  _on_destroyed_callback(_player, i);
              }

            _player->Hit(50, hitdir);
            if(!_player->IsAlive())
              {
                AddExplosion(_player->GetPosition());
                if(_on_destroyed_callback)
                  _on_destroyed_callback(i, _player);
              }
            
          }

        if(i->IsAlive())
          {
            const auto max_x = 10.0f + 0.75f;
            if(std::abs(i->GetPosition().x) > max_x)
              i->Hit(99999, -glm::normalize(i->GetVelocity()));
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
                hitdir.y = 0.0f;
                target = _player;
              }
          }

        if(target)
          {
            target->Hit(projectile->GetDamage(), hitdir);
            projectile->Hit(projectile->GetDamage(), -hitdir);

            if(!target->IsAlive())
              {
                AddExplosion(target->GetPosition());
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

  for(auto w : _walls)
    w->Tick(deltatime);
}


void Scene::AddExplosion(const glm::vec3 & position)
{
  for(unsigned int i = 0; i < _explosions.size(); i++)
    {
      auto explosion = _explosions[_explosionpos];
      
      _explosionpos++;
      if(_explosionpos >= _explosions.size())
        _explosionpos = 0;
      
      if(!explosion->IsAlive())
        {
          explosion->Activate(position);
          break;
        }
    }
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

