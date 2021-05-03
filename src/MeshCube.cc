#include "MeshCube.hh"
#include "SubsystemAssetLoader.hh"

MeshCube::MeshCube()
  : Mesh()
{
  glm::vec3 vertices[] =
    {
#if 1
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
#else
      glm::vec3(-0.5, -0.5, -0.5),
      glm::vec3(-0.5, -0.5,  0.5),
      glm::vec3( 0.5, -0.5,  0.5),
      glm::vec3( 0.5, -0.5, -0.5),
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

  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
}



MeshCube2::MeshCube2()
  : Mesh()
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
  
  this->SetPrimitiveType(GL_QUADS);
  this->Update();

  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
}


MeshCube3::MeshCube3()
  : Mesh()
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
  
  this->SetPrimitiveType(GL_QUADS);
  this->Update();

  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
}


MeshCube4::MeshCube4()
  : Mesh()
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
  
  this->SetPrimitiveType(GL_QUADS);
  this->Update();

  this->SetShaderProgram(AssetLoader->LoadShaderProgram("x"));
}
