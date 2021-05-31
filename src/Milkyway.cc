#include "Milkyway.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


Milkyway::Milkyway()
{
  _mesh = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
  _mesh->SetTexture(AssetLoader->LoadImage("8k_stars_milky_way"));
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));

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
    _mesh->AddVertex(v);
  for(auto tc : texcoords)
    _mesh->AddTexCoord(tc);
  for(auto i : indices)
    _mesh->AddElement(i);
    
  _mesh->UpdateGPU();    
}


void Milkyway::Draw(const Camera & camera) const
{
  glm::mat4 model(1);
  model = glm::translate(model, glm::vec3(0, 0, 0.5));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
  model = glm::scale(model, glm::vec3(4, 2, 2));

  glm::mat4 proj = camera.GetProjection();

  glm::mat4 view = glm::lookAt(glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  
  _mesh->Draw(model, view, proj, proj * view * model);
}
