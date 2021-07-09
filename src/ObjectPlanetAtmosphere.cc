#include "ObjectPlanetAtmosphere.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


ObjectPlanetAtmosphere::ObjectPlanetAtmosphere(Scene * scene, const glm::vec2 & size, const glm::vec3 & bottom_color, const glm::vec3 & top_color)
  : Object(scene)
{
  {
    auto mesh = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_COLOR);
    mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));

    std::vector<glm::vec3> vertices {
      glm::vec3( 1, 0,  1), 
      glm::vec3( 1, 0, -1), 
      glm::vec3(-1, 0, -1),
      glm::vec3(-1, 0,  1),
    };
    std::vector<unsigned int> indices {
      0, 3, 1,
      1, 3, 2
    };
    std::vector<glm::vec3> colors {
      top_color,
      bottom_color,
      bottom_color,
      top_color
    };
    for(auto v : vertices)
      mesh->AddVertex(v * glm::vec3(size.x * 0.5f, 1, size.y * 0.5f));
    for(auto c : colors)
      mesh->AddColor(c);
    for(auto i : indices)
      mesh->AddElement(i);
    
    mesh->UpdateGPU();

    SetMesh(mesh);
  }

  SetUseHealth(false);
}
