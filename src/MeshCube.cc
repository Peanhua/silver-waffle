#include "MeshCube.hh"

MeshCube::MeshCube()
  : Mesh()
{
  glm::vec3 vertices[] =
    {
#if 1
      // bottom
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(1, 1, 0),
      glm::vec3(1, 0, 0),

      // top
      glm::vec3(0, 0, 1),
      glm::vec3(1, 0, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(0, 1, 1),

      // back
      glm::vec3(0, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(1, 0, 1),
      glm::vec3(0, 0, 1),

      // front
      glm::vec3(0, 1, 0),
      glm::vec3(0, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 0),
#else
      glm::vec3(0, 0, 0),
      glm::vec3(0, 0, 1),
      glm::vec3(1, 0, 1),
      glm::vec3(1, 0, 0),
#endif
    };
  glm::vec3 colors[] =
    {
#if 1
      // red
      glm::vec3(1, 0.0, 0.0),
      glm::vec3(1, 0.1, 0.1),
      glm::vec3(1, 0.2, 0.2),
      glm::vec3(1, 0.3, 0.3),

      // green
      glm::vec3(0.0, 1, 0.0),
      glm::vec3(0.1, 1, 0.1),
      glm::vec3(0.2, 1, 0.2),
      glm::vec3(0.3, 1, 0.3),

      // blue
      glm::vec3(0.0, 0.0, 1),
      glm::vec3(0.1, 0.1, 1),
      glm::vec3(0.2, 0.2, 1),
      glm::vec3(0.3, 0.3, 1),

      // yellow
      glm::vec3(1, 1, 0.0),
      glm::vec3(1, 1, 0.1),
      glm::vec3(1, 1, 0.2),
      glm::vec3(1, 1, 0.3),
#else
      glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(1, 0, 0),
#endif
    };
  for(auto v : vertices)
    this->AddVertex(v * 0.9f);
  for(auto c : colors)
    this->AddColor(c);
  
  this->SetPrimitiveType(GL_QUADS);
  this->Update();
}
