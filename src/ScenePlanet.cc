#include "ScenePlanet.hh"
#include "ObjectSpaceship.hh"


ScenePlanet::ScenePlanet()
  : Scene({24, 0, 5}, {true, false, false})
{
}


void ScenePlanet::SetupPlayer()
{
  Scene::SetupPlayer();
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


void ScenePlanet::SetupSceneObject(Object * object, bool destroy_on_block)
{
  const auto m = GetPlayAreaSize();
  const auto low  = glm::vec3(-m.x * 0.5f, 1, -m.z * 0.5f);
  const auto high = glm::vec3( m.x * 0.5f, 0,  m.z * 0.5f);
  object->SetAutoDestroyBox(low, high);
  
  Object::ExceedAction blockaction = destroy_on_block ? Object::ExceedAction::DESTROY : Object::ExceedAction::STOP;
  for(unsigned int i = 0; i < 3; i++)
    object->SetOnExceedingPlayAreaLimits(i, _play_area_wraps[i] ? Object::ExceedAction::WRAP : blockaction);

  auto d = dynamic_cast<ObjectMovable *>(object);
  if(d)
    d->EnableVelocity(true, false, true);
}
