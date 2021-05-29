#include "Level.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "glm.hh"
#include <cassert>
#include <GL/glew.h>


Level::Level(const std::string & planet_texture, double distance_multiplier)
  : _distance_multiplier(distance_multiplier),
    _planet_position(0),
    _planet_rotation(0)
{
  _planet = AssetLoader->LoadMesh("Planet");
  assert(_planet);
  _planet_texture = AssetLoader->LoadImage(planet_texture);
}


void Level::Tick(double deltatime)
{
  _planet_rotation += 1.5 * deltatime;
  if(_planet_rotation > 360.0)
    _planet_rotation -= 360.0;

  _planet_position -= 3.0 * Settings->GetDouble("cheat_planet_movement_multiplier") / _distance_multiplier * deltatime;
}


bool Level::IsFinished() const
{
  return _planet_position < -2080;
}


void Level::Draw(const Camera & camera) const
{
  glDisable(GL_DEPTH_TEST);
  glm::mat4 model(1);
  model = glm::translate(model, glm::vec3(0, 2000 + _planet_position, -101));
  model = glm::rotate(model, static_cast<float>(glm::radians(_planet_rotation)), glm::vec3(0, 0, 1));
  model = glm::scale(model, glm::vec3(200, 200, 200));
  _planet->SetTexture(_planet_texture, true);
  _planet->Draw(model, camera.GetView(), camera.GetProjection(), camera.GetViewProjection() * model);
}


void Level::Start()
{
  _planet_position = 0;
}
