#include "SpaceParticles.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"


SpaceParticles::SpaceParticles(double radius_min, double radius_max, unsigned long random_seed)
  : _radius_min(radius_min),
    _radius_max(radius_max),
    _time(0),
    _add_star_timer(0),
    _star_count(400),
    _next_index(0),
    _random(random_seed),
    _rdist(0, 1)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_BLEND | Mesh::OPTION_VERTEX_W | Mesh::OPTION_GENERIC_VEC2_INPUT | Mesh::OPTION_DYNAMIC_VERTEX);
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Star"));
  _mesh->SetPrimitiveType(GL_POINTS);
  const double min_speed = 5.0;
  const double max_speed = 30.0;
  for(unsigned int i = 0; i < _star_count; i++)
    {
      _mesh->AddVertex(glm::vec4(0, -100, 0, 0));
      _mesh->AddColor(glm::vec3(GetRandom(0.65), GetRandom(0.65), GetRandom(0.65)));

      double speed = min_speed + GetRandom() * (max_speed - min_speed);
      _mesh->AddGenericVecInput(glm::vec2(speed, GetRandom()));
    }
  _mesh->UpdateGPU();

  for(int i = 0; i < 60 * 30; i++)
    Tick(1.0 / 60.0);
}


double SpaceParticles::GetRandom(double min)
{
  return min + (1.0 - min) * _rdist(_random);
}


void SpaceParticles::Tick(double deltatime)
{
  _time += deltatime;

  if(_add_star_timer < 0.0)
    {
      for(int i = 0; i < 10; i++)
        {
          const auto v = _mesh->GetVertex4(_next_index);
          const auto g = _mesh->GetGenericVec2(_next_index);
          if(v.y - g.x * (static_cast<float>(_time) - v.w) < 40.0f - 53.0f - 2.0f)
            {
              ResetStar(_next_index);
              _add_star_timer += 0.05;
              break;
            }
  
          _next_index++;
          if(_next_index >= _star_count)
            _next_index = 0;
        }
    }
  else
    _add_star_timer -= deltatime;
}


void SpaceParticles::ResetStar(unsigned int index)
{
  double angle = GetRandom() * glm::radians(360.0);
  auto pos = glm::vec3(_radius_min + GetRandom() * (_radius_max - _radius_min),
                       150.0,
                       0);
  auto rot = glm::rotate(glm::mat4(1), static_cast<float>(angle), glm::vec3(0, 1, 0));
  pos = rot * glm::vec4(pos, 1);

  _mesh->SetVertex(index, glm::vec4(pos, _time));
  _mesh->UpdateGPU(Mesh::OPTION_VERTEX, index, 1);
}


void SpaceParticles::Draw(const Camera & camera) const
{
  const glm::mat4 & view       = camera.GetView();
  const glm::mat4 & projection = camera.GetProjection();
  const glm::mat4 & mvp        = camera.GetViewProjection();
    
  glm::mat4 model(1);
  model = glm::translate(model, glm::vec3(0, 40 - 53 - 2, 0));

  _mesh->GetShaderProgram()->Use();
  _mesh->GetShaderProgram()->SetFloat("in_time", static_cast<float>(_time));

  _mesh->Draw(model, view, projection, mvp * model);
}
