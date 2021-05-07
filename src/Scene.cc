#include "Scene.hh"
#include "ObjectInvader.hh"
#include "ObjectBullet.hh"
#include "SubsystemAssetLoader.hh"
#include <iostream>


Scene::Scene()
  : Object(glm::vec3(0, 0, 0)),
    _bulletpos(0)
{
  for(int i = 0; i < 1000; i++)
    {
      auto b = new ObjectBullet();
      _bullets.push_back(b);
      AddChild(b);
    }
}


void Scene::Draw(const glm::mat4 & mvp) const
{
  const glm::mat4 mymvp(mvp * glm::translate(glm::mat4(1.0f), GetPosition()));

  if(GetMesh())
    GetMesh()->Draw(mymvp);

  _player->Draw(mymvp);

  for(auto i : _invaders)
    i->Draw(mymvp);

  for(auto b : _bullets)
    b->Draw(mymvp);

}


void Scene::Initialize(double difficulty)
{
  assert(difficulty == difficulty);
  
  glm::vec3 topleft(-10, 0, 40);

  _player = new ObjectMovable(topleft + glm::vec3(0, 0, -50));
  {
    auto mesh = AssetLoader->LoadMesh("Player");
    assert(mesh);
    _player->SetMesh(mesh);
  }
  AddChild(_player);
  
  _invaders.clear();
  for(int y = 0; y < 10; y++)
    for(int x = 0; x < 20; x++)
      {
        auto invader = new ObjectInvader(topleft + glm::vec3(x, 0, -y));

        auto mesh = AssetLoader->LoadMesh("Invader1");
        assert(mesh);
        invader->SetMesh(mesh);
        
        AddChild(invader);
        _invaders.push_back(invader);
      }
}


ObjectMovable * Scene::GetPlayer() const
{
  return _player;
}


void Scene::AddPlayerBullet(const glm::vec3 & velocity, double lifetime)
{
  bool done = false;
  for(unsigned int i = 0; !done && i < _bullets.size(); i++)
    {
      if(!_bullets[_bulletpos]->IsAlive())
        {
          _bullets[_bulletpos]->Activate(_player->GetPosition(), _player->GetVelocity() * 0.5f + velocity, lifetime);
          done = true;
        }

      _bulletpos++;
      if(_bulletpos >= _bullets.size())
        _bulletpos = 0;
    }
}


void Scene::Tick(double deltatime)
{
  Object::Tick(deltatime);

  for(auto b : _bullets)
    if(b->IsAlive())
      for(auto i : _invaders)
        if(i->IsAlive())
          {
            glm::vec3 hitdir;
            if(b->CheckCollision(*i, hitdir))
              {
                //hitdir = glm::normalize(hitdir + b->GetVelocity() /* + i->GetVelocity() */);
                i->Hit(34, hitdir * 2.0f);
                b->Hit(34, -hitdir * 2.0f);
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

