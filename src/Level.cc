#include "Level.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "glm.hh"
#include <cassert>



Level::Level(Scene * scene, const std::string & planet_texture, double planet_size)
  : _scene(scene),
    _random_generator(0),
    _planet_size(planet_size),
    _planet_position_start(200.0 + planet_size * 9.0),
    _planet_position(0),
    _planet_rotation(0)
{
  _planet = AssetLoader->LoadMesh("Planet");
  assert(_planet);
  _planet_texture = AssetLoader->LoadImage(planet_texture);

  auto end_of_time = _planet_position_start - 2.0 * (10 + 53.0); // 2.0 is the speed of an invader, 53 is distance from invader spawn to player, 10 extra
  
  auto e = new ProgramEntry();
  e->SetStartTime(0.0);
  e->SetStopTime(30.0);
  e->SetSpawnInterval(0.5);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(60.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(2.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(70.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(3.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(90.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(4.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(120.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(2.0);
  _program.push_back(e);

  e = new ProgramEntry();
  e->SetStartTime(end_of_time - 50.0);
  e->SetStopTime(end_of_time);
  e->SetSpawnInterval(0.05);
  _program.push_back(e);
}


void Level::Tick(double deltatime)
{
  _planet_rotation += 1.5 * deltatime;
  if(_planet_rotation > 360.0)
    _planet_rotation -= 360.0;

  _planet_position -= deltatime;

  for(auto e : _program)
    e->Tick(_scene, _random_generator, deltatime);
}


bool Level::IsFinished() const
{
  return _planet_position < -20 - _planet_position_start - _planet_size * 0.35;
}


void Level::Draw(const Camera & camera) const
{
  glDisable(GL_DEPTH_TEST);
  glm::mat4 model(1);
  model = glm::translate(model, glm::vec3(0, _planet_position_start + _planet_position, -_planet_size * 0.5));
  model = glm::rotate(model, static_cast<float>(glm::radians(_planet_rotation)), glm::vec3(0, 0, 1));
  model = glm::scale(model, glm::vec3(_planet_size, _planet_size, _planet_size));
  _planet->SetTexture(_planet_texture, true);
  _planet->Draw(model, camera.GetView(), camera.GetProjection(), camera.GetViewProjection() * model);
}


void Level::Start()
{
  _planet_position = 0;
}



Level::ProgramEntry::ProgramEntry()
  : _invader_spawn_timer(0),
    _invader_spawn_start_time(0),
    _invader_spawn_stop_time(999999),
    _invader_spawn_interval(1.0)
{
}


void Level::ProgramEntry::SetStartTime(double time)
{
  _invader_spawn_start_time = time;
}


void Level::ProgramEntry::SetStopTime(double time)
{
  _invader_spawn_stop_time = time;
}


void Level::ProgramEntry::SetSpawnInterval(double interval)
{
  _invader_spawn_interval = interval;
}


void Level::ProgramEntry::Tick(Scene * scene, std::mt19937_64 & random_generator, double deltatime)
{
  if(_invader_spawn_stop_time < 0.0)
      return;
  _invader_spawn_stop_time -= deltatime;
  
  if(_invader_spawn_start_time > 0.0)
    {
      _invader_spawn_start_time -= deltatime;
      return;
    }

  _invader_spawn_timer += deltatime;
  if(_invader_spawn_timer > _invader_spawn_interval)
    {
      _invader_spawn_timer -= _invader_spawn_interval;
      
      auto rand = [&random_generator]()
      {
        return (static_cast<float>(random_generator()) - static_cast<float>(random_generator.min())) / static_cast<float>(random_generator.max());
      };
      
      const auto max_x = scene->GetPlayAreaSize().x * 0.5f;
      scene->AddInvader(glm::vec3(-max_x + rand() * max_x * 2.0f, 40, 0));
    }
}
