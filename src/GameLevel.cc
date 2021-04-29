#include "GameLevel.hh"
#include "MeshCube.hh"
#include <iostream>

void GameLevel::Initialize(double difficulty)
{
  const int aliens_per_row = 4;
  const double spacing = 0.1;

  glm::vec3 topleft(0, 0, 0);

  for(int i = 0; i < 1 /*static_cast<int>(difficulty * 10.0)*/; i++)
    {
      glm::vec3 pos = topleft + glm::vec3(spacing * (i % aliens_per_row),
                                          spacing * (i / aliens_per_row),
                                          0.0);
      auto invader = new Object(pos);
      invader->SetMesh(new MeshCube());
      this->AddChild(invader);
    }
}

