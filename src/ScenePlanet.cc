#include "ScenePlanet.hh"
#include "ObjectSpaceship.hh"


ScenePlanet::ScenePlanet()
  : Scene({6, 1}, {true, false, false})
{
}


void ScenePlanet::SetupPlayer()
{
  auto player = GetPlayer();
  player->EnableVelocity(true, false, true);
  player->SetPosition(glm::vec3(0, 0, 0));
  player->RotateYaw(90.0);
  for(unsigned int i = 0; i < 4; i++)
    {
      player->EnableEngine(i,     false);
      player->EnableEngine(i + 4, true);
    }
}
