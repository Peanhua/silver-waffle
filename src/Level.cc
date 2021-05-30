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
    _planet_ring(nullptr),
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
  return _planet_position < -20.0 - _planet_position_start - _planet_size * 0.35;
}


void Level::Draw(const Camera & camera) const
{
  glm::mat4 model(1);
  model = glm::translate(model, glm::vec3(0, _planet_position_start + _planet_position, -_planet_size * 0.5));
  model = glm::rotate(model, static_cast<float>(glm::radians(_planet_rotation)), glm::vec3(0, 0, 1));
  model = glm::scale(model, glm::vec3(_planet_size, _planet_size, _planet_size));
  _planet->SetTexture(_planet_texture, true);
  _planet->Draw(model, camera.GetView(), camera.GetProjection(), camera.GetViewProjection() * model);
  if(_planet_ring)
    {
      //glDisable(GL_CULL_FACE);
      _planet_ring->SetTexture(AssetLoader->LoadImage("8k_saturn_ring_alpha"));
      _planet_ring->Draw(model, camera.GetView(), camera.GetProjection(), camera.GetViewProjection() * model);
      //glEnable(GL_CULL_FACE);
    }
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


void Level::SetPlanetRing(float start, float end)
{ // todo: Move planet and its ring to Scene as an ObjectMovable.
  _planet_ring = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
  _planet_ring->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));

  constexpr auto PI = 4.0f * std::atan(1.0f);
  constexpr auto step = 2.0f * PI / 128.0f;
  
  unsigned int ind = 0;
  for(auto arc = -PI; arc < PI - step; arc += step)
    {
      std::vector<glm::vec3> vertices
        {
          glm::vec3(end   * std::sin(arc + step), end   * std::cos(arc + step), 0),
          glm::vec3(start * std::sin(arc + step), start * std::cos(arc + step), 0),
          glm::vec3(start * std::sin(arc),        start * std::cos(arc),        0),
          glm::vec3(end   * std::sin(arc),        end   * std::cos(arc),        0),
        };
      for(auto v : vertices)
        _planet_ring->AddVertex(v);

      std::vector<glm::vec2> texcoords
        {
          glm::vec2(1, 1),
          glm::vec2(0, 0),
          glm::vec2(0, 1),
          glm::vec2(1, 0),
        };
      for(auto t : texcoords)
        _planet_ring->AddTexCoord(t);

      std::vector<unsigned int> indices
        {
          0, 3, 1,
          1, 3, 2,
        };
      for(auto i : indices)
        _planet_ring->AddElement(ind + i);
      ind += 4;
    }
  _planet_ring->UpdateGPU();
}

