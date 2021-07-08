#include "SceneSpace.hh"
#include "Camera.hh"
#include "Milkyway.hh"
#include "ObjectSpaceship.hh"
#include "SpaceParticles.hh"
#include "WormholeWall.hh"


SceneSpace::SceneSpace()
  : Scene({20, 60, 0}, {false, false, false})
{
  _milkyway = new Milkyway();
  _particles = new SpaceParticles(5.0, 50.0, 0);
  _wall = new WormholeWall(100, 4);
  SetupPlayer();
}


void SceneSpace::Draw(const Camera & camera) const
{
  _milkyway->Draw(camera);
  Scene::Draw(camera);

  // Two walls (left and right), todo: test doing a half&full circle of walls, maybe left&right normally, and half/full circle during bonus levels?
  const glm::mat4 & view       = camera.GetView();
  const glm::mat4 & projection = camera.GetProjection();
  const glm::mat4 & vp         = camera.GetViewProjection();

  const auto max_x = GetPlayAreaSize().x * 0.5f + 0.75f; // 0.75 is approximately half of the player ship width, so that the ship never goes through the wall
  for(int i = 0; i < 360; i += 180)
    {
      glm::mat4 model(1);
      model = glm::rotate(model, glm::radians(static_cast<float>(i)), glm::vec3(0, 1, 0));
      model = glm::translate(model, glm::vec3(-max_x, 40 - 53 - 1, -4/2));
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
      _wall->Draw(model, view, projection, vp * model);
    }
}

  
void SceneSpace::Tick(double deltatime)
{
  Scene::Tick(deltatime);
  _wall->Tick(deltatime);
}


void SceneSpace::SetupPlayer()
{
  Scene::SetupPlayer();
  auto player = GetPlayer();
  assert(player);
  player->EnableVelocity(true, false, false);
  player->SetPosition(glm::vec3(0, 40 - 53, 0));
  for(unsigned int i = 0; i < 4; i++)
    {
      player->EnableEngine(i,     true);
      player->EnableEngine(i + 4, false);
    }
}


void SceneSpace::SetupSceneObject(Object * object, bool destroy_on_block)
{
  const auto m = GetPlayAreaSize();
  const double bsr = object->GetVisualBoundingSphereRadius();
  
  const auto low  = glm::vec3(-m.x * 0.5f, 40 - 53 - 2 - bsr, 1);
  const auto high = glm::vec3( m.x * 0.5f,           9999999, 0);
  object->SetAutoDestroyBox(low, high);
  
  Object::ExceedAction blockaction = destroy_on_block ? Object::ExceedAction::DESTROY : Object::ExceedAction::STOP;
  for(unsigned int i = 0; i < 3; i++)
    object->SetOnExceedingPlayAreaLimits(i, _play_area_wraps[i] ? Object::ExceedAction::WRAP : blockaction);

  auto d = dynamic_cast<ObjectMovable *>(object);
  if(d)
    d->EnableVelocity(true, true, false);
}


glm::vec3 SceneSpace::GetRandomSpawnPosition()
{
  auto rand = [this]()
  {
    return _rdist(_random_generator);
  };

  const auto max_x = GetPlayAreaSize().x * 0.5f;
  return glm::vec3(-max_x + rand() * max_x * 2.0f, 40, 0);
}
