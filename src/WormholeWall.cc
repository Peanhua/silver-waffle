#include "WormholeWall.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>


WormholeWall::WormholeWall(double length, double height)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT);
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
  
  for(auto v : vertices)
    {
      _mesh->AddVertex(v);
      //_mesh->AddColor(glm::vec3(0, 0, 0));
      _mesh->AddColor(0.1f * glm::vec3(0.64, 0.12, 0.77));
    }
  for(auto ii : indices)
    _mesh->AddElement(ii);
  
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
