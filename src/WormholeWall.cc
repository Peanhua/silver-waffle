#include "WormholeWall.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>


WormholeWall::WormholeWall(double length, double height)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT | Mesh::OPTION_COLOR_ALPHA | Mesh::OPTION_BLEND);
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
  
  std::vector<glm::vec3> vertices {
    glm::vec3(length, 0, height), // 0
    glm::vec3(length, 0,      0), // 1
    glm::vec3(0,      0,      0), // 2
    glm::vec3(0,      0, height), // 3
  };
  
  std::vector<unsigned int> indices {
    0, 3, 1,
    1, 3, 2,
  };
  std::vector<glm::vec4> colors {
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.0f),
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.0f),
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.5f),
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.5f),
  };
  
  for(auto v : vertices)
    _mesh->AddVertex(v);
  for(auto c : colors)
    _mesh->AddColor(c);
  for(auto i : indices)
    _mesh->AddElement(i);
  
  _mesh->UpdateGPU();
}


void WormholeWall::Tick(double deltatime)
{
  assert(deltatime == deltatime);
}


void WormholeWall::Draw(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & mvp) const
{
  _mesh->Draw(model, view, projection, mvp);
}
