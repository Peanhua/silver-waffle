#include "MeshCube.hh"
#include "SubsystemAssetLoader.hh"

MeshCube::MeshCube()
  : Mesh(OPTION_COLOR | OPTION_ELEMENT)
{
  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));

#if 0
  glm::vec3 vertices[] =
    {
      glm::vec3(-1, 1, 1 ),
      glm::vec3(-0.370879, -0.358338, 2.24083 ),
      glm::vec3(1, 1, 1 ),
      glm::vec3(-1, -1, 1 ),
      glm::vec3(-0.370879, -0.915833, 2.24083 ),
      glm::vec3(-0.928374, -0.915833, 2.24083 ),
      glm::vec3(-1, -1, 1 ),
      glm::vec3(-0.928374, -0.358338, 2.24083 ),
      glm::vec3(-1, 1, 1 ),
      glm::vec3(1, -1, 1 ),
      glm::vec3(-0.370879, -0.358338, 2.24083 ),
      glm::vec3(-0.370879, -0.915833, 2.24083 ),
      glm::vec3(-1, 1, 1 ),
      glm::vec3(-0.928374, -0.358338, 2.24083 ),
      glm::vec3(-0.370879, -0.358338, 2.24083 ),
      glm::vec3(-1, -1, 1 ),
      glm::vec3(1, -1, 1 ),
      glm::vec3(-0.370879, -0.915833, 2.24083 ),
      glm::vec3(-1, -1, 1 ),
      glm::vec3(-0.928374, -0.915833, 2.24083 ),
      glm::vec3(-0.928374, -0.358338, 2.24083 ),
      glm::vec3(1, -1, 1 ),
      glm::vec3(1, 1, 1 ),
      glm::vec3(-0.370879, -0.358338, 2.24083 ),
    };

  int indices[] =
    {
      0, 1, 2,
      3, 4, 5,
      6, 7, 8,
      9, 10, 11,
      12, 13, 14,
      15, 16, 17,
      18, 19, 20,
      21, 22, 23,
    };
  
  for(auto v : vertices)
    {
      this->AddVertex(v * 0.9f);
      AddColor(glm::vec3(1, 1, 1));
    }
  for(auto i : indices)
    this->AddElement(i);
#else
  LoadFromFile("test.dae");
#endif
  this->UpdateGPU();
  this->CalculateBoundingSphereRadius();
}



MeshCube2::MeshCube2()
  : Mesh(OPTION_COLOR)
{
  glm::vec3 vertices[] =
    {
      // bottom
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3(-0.5,  0.5, -0.5),
      glm::vec3( 0.5,  0.5, -0.5),
      glm::vec3( 0.5, -0.5, -0.5),

      // top
      glm::vec3(-0.5, -0.5, 0.5),
      glm::vec3( 0.5, -0.5, 0.5),
      glm::vec3( 0.5,  0.5, 0.5),
      glm::vec3(-0.5,  0.5, 0.5),

      // back
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3( 0.5, -0.5, -0.5),
      glm::vec3( 0.5, -0.5,  0.5),
      glm::vec3(-0.5, -0.5,  0.5),

      // front
      glm::vec3(-0.5, 0.5, -0.5),
      glm::vec3(-0.5, 0.5,  0.5),
      glm::vec3( 0.5, 0.5,  0.5),
      glm::vec3( 0.5, 0.5, -0.5),
    };
  glm::vec3 colors[] =
    {
      // red
      glm::vec3(1, 0.0, 0.0),
      glm::vec3(1, 0.1, 0.1),
      glm::vec3(1, 0.2, 0.2),
      glm::vec3(1, 0.3, 0.3),

      // red
      glm::vec3(1, 0.0, 0.0),
      glm::vec3(1, 0.1, 0.1),
      glm::vec3(1, 0.2, 0.2),
      glm::vec3(1, 0.3, 0.3),

      // red
      glm::vec3(1, 0.0, 0.0),
      glm::vec3(1, 0.1, 0.1),
      glm::vec3(1, 0.2, 0.2),
      glm::vec3(1, 0.3, 0.3),

      // red
      glm::vec3(1, 0.0, 0.0),
      glm::vec3(1, 0.1, 0.1),
      glm::vec3(1, 0.2, 0.2),
      glm::vec3(1, 0.3, 0.3),
    };
  for(auto v : vertices)
    this->AddVertex(v * 0.9f);
  for(auto c : colors)
    this->AddColor(c);
  
  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
  this->SetPrimitiveType(GL_QUADS);
  this->UpdateGPU();
  this->CalculateBoundingSphereRadius();
}


MeshCube3::MeshCube3()
  : Mesh(OPTION_COLOR)
{
  glm::vec3 vertices[] =
    {
      // bottom
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3(-0.5,  0.5, -0.5),
      glm::vec3( 0.5,  0.5, -0.5),
      glm::vec3( 0.5, -0.5, -0.5),

      // top
      glm::vec3(-0.5, -0.5, 0.5),
      glm::vec3( 0.5, -0.5, 0.5),
      glm::vec3( 0.5,  0.5, 0.5),
      glm::vec3(-0.5,  0.5, 0.5),

      // back
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3( 0.5, -0.5, -0.5),
      glm::vec3( 0.5, -0.5,  0.5),
      glm::vec3(-0.5, -0.5,  0.5),

      // front
      glm::vec3(-0.5, 0.5, -0.5),
      glm::vec3(-0.5, 0.5,  0.5),
      glm::vec3( 0.5, 0.5,  0.5),
      glm::vec3( 0.5, 0.5, -0.5),
    };
  glm::vec3 colors[] =
    {
      // green
      glm::vec3(0.0, 1, 0.0),
      glm::vec3(0.1, 1, 0.1),
      glm::vec3(0.2, 1, 0.2),
      glm::vec3(0.3, 1, 0.3),

      // green
      glm::vec3(0.0, 1, 0.0),
      glm::vec3(0.1, 1, 0.1),
      glm::vec3(0.2, 1, 0.2),
      glm::vec3(0.3, 1, 0.3),

      // green
      glm::vec3(0.0, 1, 0.0),
      glm::vec3(0.1, 1, 0.1),
      glm::vec3(0.2, 1, 0.2),
      glm::vec3(0.3, 1, 0.3),

      // green
      glm::vec3(0.0, 1, 0.0),
      glm::vec3(0.1, 1, 0.1),
      glm::vec3(0.2, 1, 0.2),
      glm::vec3(0.3, 1, 0.3),
    };
  for(auto v : vertices)
    this->AddVertex(v * 0.9f);
  for(auto c : colors)
    this->AddColor(c);
  
  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
  this->SetPrimitiveType(GL_QUADS);
  this->UpdateGPU();
  this->CalculateBoundingSphereRadius();
}


MeshCube4::MeshCube4()
  : Mesh(OPTION_COLOR)
{
  glm::vec3 vertices[] =
    {
      // bottom
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3(-0.5,  0.5, -0.5),
      glm::vec3( 0.5,  0.5, -0.5),
      glm::vec3( 0.5, -0.5, -0.5),

      // top
      glm::vec3(-0.5, -0.5, 0.5),
      glm::vec3( 0.5, -0.5, 0.5),
      glm::vec3( 0.5,  0.5, 0.5),
      glm::vec3(-0.5,  0.5, 0.5),

      // back
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3( 0.5, -0.5, -0.5),
      glm::vec3( 0.5, -0.5,  0.5),
      glm::vec3(-0.5, -0.5,  0.5),

      // front
      glm::vec3(-0.5, 0.5, -0.5),
      glm::vec3(-0.5, 0.5,  0.5),
      glm::vec3( 0.5, 0.5,  0.5),
      glm::vec3( 0.5, 0.5, -0.5),
    };
  glm::vec3 colors[] =
    {
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),

      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),

      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),

      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
      glm::vec3(1, 1, 1),
    };
  for(auto v : vertices)
    this->AddVertex(v * 0.1f);
  for(auto c : colors)
    this->AddColor(c);
  
  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
  this->SetPrimitiveType(GL_QUADS);
  this->UpdateGPU();
  this->CalculateBoundingSphereRadius();
}
