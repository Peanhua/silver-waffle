#include "SceneSpace.hh"
#include "Camera.hh"
#include "Milkyway.hh"
#include "SpaceParticles.hh"
#include "WormholeWall.hh"


SceneSpace::SceneSpace()
  : Scene()
{
  _milkyway = new Milkyway();
  _particles = new SpaceParticles(5.0, 50.0, 0);
  _wall = new WormholeWall(100, 4);

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
