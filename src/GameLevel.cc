#include "GameLevel.hh"
#include "MeshCube.hh"
#include <iostream>

void GameLevel::Initialize(double difficulty)
{
  assert(difficulty == difficulty);
  
  glm::vec3 topleft(-6, 0, 3);

  for(int y = 0; y < 8; y++)
    for(int x = 0; x < 12; x++)
      {
        auto invader = new Object(topleft + glm::vec3(x, 0, -y));
        invader->SetMesh(new MeshCube());
        this->AddChild(invader);
      }
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
        invader->SetMesh(new MeshCube());
        this->AddChild(invader);
      }
}

