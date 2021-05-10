#include "MeshOverlay.hh"
#include "Image.hh"
#include "SubsystemAssetLoader.hh"
#include <vector>

MeshOverlay::MeshOverlay()
  : Mesh(OPTION_ELEMENT | OPTION_TEXTURE | OPTION_BLEND)
{
  auto texture = new Image(true);
  texture->Load("Images/GameOverlay.png");
  texture->UpdateGPU(true, true);
  SetTexture(texture);
  assert(GetTexture());

  SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));

  std::vector<glm::vec3> vertices {
    glm::vec3( 1,  1, 0), 
    glm::vec3( 1, -1, 0), 
    glm::vec3(-1, -1, 0),
    glm::vec3(-1,  1, 0),
  };

  std::vector<glm::vec2> texcoords {
    glm::vec2(1, 1),
    glm::vec2(1, 0),
    glm::vec2(0, 0),
    glm::vec2(0, 1) 
  };

  std::vector<unsigned int> indices {
    0, 3, 1,
    1, 3, 2
  };

  for(auto v : vertices)
    AddVertex(v);

  for(auto tc : texcoords)
    AddTexCoord(tc);

  for(auto i : indices)
    AddElement(i);
  
  UpdateGPU();
}
