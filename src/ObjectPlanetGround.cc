#include "ObjectPlanetGround.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


ObjectPlanetGround::ObjectPlanetGround(Scene * scene, const glm::vec2 & size, Image * texture_map)
  : Object(scene)
{
  AddToCollisionChannel(Object::CollisionChannel::TERRAIN);

  {
    auto mesh = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_COLOR | Mesh::OPTION_TEXTURE | Mesh::OPTION_NORMAL);
    mesh->SetTexture(0, texture_map);
    mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("SceneObject-Texture"));

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
      {
        mesh->AddVertex(v * glm::vec3(size * 0.5f, 1));
        mesh->AddColor({1.0, 1.0, 1.0});
        mesh->AddNormal({0, 0, 1});
      }
    for(auto tc : texcoords)
      mesh->AddTexCoord(tc);
    for(auto i : indices)
      mesh->AddElement(i);
    
    mesh->UpdateGPU();

    SetMesh(mesh);
  }

  SetUseHealth(false);
}


void ObjectPlanetGround::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  other.Hit(this, 100, -hit_direction);
}