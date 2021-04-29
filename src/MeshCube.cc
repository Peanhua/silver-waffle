#include "MeshCube.hh"

MeshCube::MeshCube()
  : Mesh()
{
  glm::vec3 vertices[] =
    {
      glm::vec3(0, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(1, 1, 0),
      glm::vec3(0, 1, 0)
    };
  glm::vec3 colors[] =
    {
      glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(0, 0, 1),
      glm::vec3(1, 1, 0),
    };
  for(auto v : vertices)
    this->AddVertex(v);
  for(auto c : colors)
    this->AddColor(c);
  
  this->SetPrimitiveType(GL_QUADS);
  this->Update();
}
