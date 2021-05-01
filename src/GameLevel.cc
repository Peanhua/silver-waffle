#include "GameLevel.hh"
#include "MeshCube.hh"
#include <iostream>

void GameLevel::Initialize(double difficulty)
{
  const int aliens_per_row = 4;
  const double spacing = 0.1;

  glm::vec3 topleft(-6, 0, 3);


  for(int y = 0; y < 8; y++)
    for(int x = 0; x < 12; x++)
      {
        auto invader = new Object(topleft + glm::vec3(x, 0, -y));
        invader->SetMesh(new MeshCube());
        this->AddChild(invader);
      }
      
  
  /*
  for(int i = 0; i < static_cast<int>(difficulty * 10.0); i++)
    {
      glm::vec3 pos = topleft + glm::vec3(spacing * (i % aliens_per_row),
                                          spacing * (i / aliens_per_row),
                                          0.0);
      auto invader = new Object(pos);
      invader->SetMesh(new MeshCube());
      this->AddChild(invader);
    }
  */

#if 0
  {
    auto invader = new Object(topleft + glm::vec3(0, 0, 1));
    invader->SetMesh(new MeshCube());
    this->AddChild(invader);
  }

  {
    auto invader = new Object(topleft + glm::vec3(0.5, 0, 1.5));
    invader->SetMesh(new MeshCube());
    this->AddChild(invader);
  }
#endif
}

