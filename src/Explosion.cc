#include "Explosion.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include <iostream>


Explosion::Explosion(std::minstd_rand & random)
  : _time(999)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_BLEND | Mesh::OPTION_GENERIC_VEC3_INPUT);

  auto rand = [&random]() { return static_cast<double>(random() - random.min()) / static_cast<double>(random.max()); };

  const int rounds = 10;
  _fragment_count = rounds * (10 + static_cast<int>(40.0 * rand()));
  for(int i = 0; i < _fragment_count / rounds; i++)
    for(int j = 1; j <= rounds; j++)
      {
        float f = static_cast<float>(j) / static_cast<float>(rounds);
        _mesh->AddGenericVecInput(glm::normalize(glm::vec3(rand() - 0.5, rand() - 0.5, rand() - 0.5)) * f);
        _mesh->AddColor(glm::vec3(rand(), rand(), rand()));
      }
  _mesh->SetPrimitiveType(GL_POINTS);
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Explosion"));
}


void Explosion::Activate(const glm::vec3 & position, const glm::vec3 & velocity)
{
  _time = 0;
  _mesh->ClearVertices();
  for(int i = 0; i < _fragment_count; i++)
    _mesh->AddVertex(position);
  _mesh->UpdateGPU();

  _velocity = velocity;
}


void Explosion::Tick(double deltatime)
{
  _time += deltatime;
}


bool Explosion::IsAlive() const
{
  return _time < 2.0;
}


void Explosion::Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const
{
  _mesh->GetShaderProgram()->Use();
  _mesh->GetShaderProgram()->SetFloat("in_time", static_cast<float>(_time));
  _mesh->GetShaderProgram()->SetVec3("in_velocity", _velocity);
  _mesh->Draw(glm::mat4(1), view, projection, vp);
}

