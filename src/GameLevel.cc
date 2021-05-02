#include "GameLevel.hh"
#include "MeshCube.hh"
#include "ObjectInvader.hh"
#include <iostream>

void GameLevel::Initialize(double difficulty)
{
  assert(difficulty == difficulty);
  
  glm::vec3 topleft(-6, 0, 3);

  _player = new ObjectMovable(topleft + glm::vec3(0, 0, -15));
  _player->SetMesh(new MeshCube3());
  AddChild(_player);
  
  _invaders.clear();
  for(int y = 0; y < 8; y++)
    for(int x = 0; x < 12; x++)
      {
        auto invader = new ObjectInvader(topleft + glm::vec3(x, 0, -y));
        if((x + y) % 3 == 0)
          invader->SetMesh(new MeshCube2());
        else
          invader->SetMesh(new MeshCube());
        AddChild(invader);
        _invaders.push_back(invader);
      }
}


ObjectMovable * GameLevel::GetPlayer() const
{
  return _player;
}


void GameLevel2::Initialize(double difficulty)
{
  assert(difficulty == difficulty);
  
  glm::vec3 zeropos(0, 0, 4);

  for(int y = 0; y < 8; y++)
    for(int x = 0; x < 12; x++)
      {
        glm::vec3 pos = glm::rotateY(zeropos, static_cast<float>(glm::radians(360.0 / 12.0 * static_cast<double>(x))));
        pos.y = y;
        auto invader = new Object(pos);
        if((x + y) % 3 == 0)
          invader->SetMesh(new MeshCube2());
        else
          invader->SetMesh(new MeshCube());
        AddChild(invader);
      }
}

