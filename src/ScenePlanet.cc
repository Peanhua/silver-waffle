#include "ScenePlanet.hh"
#include "Camera.hh"
#include "ControllerPlanet.hh"
#include "ObjectPlanetAtmosphere.hh"
#include "ObjectPlanetGround.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"


ScenePlanet::ScenePlanet()
  : Scene({100, 0, 20}, {true, false, false}),
    _landing_sequence(true)
{
  auto ground = new ObjectPlanetGround(this, {100, 40}, AssetLoader->LoadImage("8k_earth_daymap"));
  AddObject(ground, {0, 0, -10});

  auto atmosphere = new ObjectPlanetAtmosphere(this, {100, 20}, {1, 0, 0}, {0, 0, 0.5});
  atmosphere->Translate({0, 20, 0});
  AddPlanet(atmosphere);
}


void ScenePlanet::SetupPlayer()
{
  Scene::SetupPlayer();
  auto player = GetPlayer();
  player->EnableVelocity(true, false, true);
  player->SetPosition({50, 0, GetPlayAreaSize().z * 0.5f});
  player->RotateYaw(-90.0);
  for(unsigned int i = 0; i < 4; i++)
    {
      player->EnableEngine(i,     false);
      player->EnableEngine(i + 4, true);
    }
  player->SetController(new ControllerPlanet(player));
  if(_landing_sequence)
    {
      _landing_sequence = false;
      player->AddImpulse({300, 0, -10});
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


glm::vec3 ScenePlanet::GetRandomSpawnPosition()
{
  auto rand = [this]()
  {
    return _rdist(_random_generator);
  };

  // Spawn in front of player at random z on air.
  glm::vec4 offset(0, 5, 0, 1);

  glm::mat4 rot = glm::toMat4(glm::inverse(GetPlayer()->GetOrientation()));
  offset = offset * rot;
  
  float groundz = 0; // todo
  float emptytop = GetPlayAreaSize().z * 0.333f;
  offset.z =
    groundz
    + rand() * (GetPlayAreaSize().z - groundz - emptytop)
    - (GetPlayAreaSize().z * 0.5f);
  
  return glm::vec3(GetPlayer()->GetPosition().xy(), 0) + offset.xyz();
}


void ScenePlanet::Draw(const Camera & camera) const
{
  Scene::Draw(camera);

  // Draws the same scene on both sides so that when player is near the edge, the other side can be seen.
  // Todo: Fix to draw at most 1 times instead of 3.
  {
    int i = -1;
    auto cam2 = new Camera(camera);
    cam2->SetPosition(cam2->GetPosition() + glm::vec3(i * 100, 0, 0));
    cam2->SetTargetPosition(cam2->GetTargetPosition() + glm::vec3(i * 100, 0, 0));
    Scene::Draw(*cam2);
  }
  {
    int i = 1;
    auto cam2 = new Camera(camera);
    cam2->SetPosition(cam2->GetPosition() + glm::vec3(i * 100, 0, 0));
    cam2->SetTargetPosition(cam2->GetTargetPosition() + glm::vec3(i * 100, 0, 0));
    Scene::Draw(*cam2);
  }
}
