#include "ObjectPlanet.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>


ObjectPlanet::ObjectPlanet(Scene * scene, Image * planet_texture, double planet_size)
  : ObjectMovable(scene)
{
  auto planet = new Mesh(*AssetLoader->LoadMesh("Planet", "Generic"));
  assert(planet);
  planet->ApplyTransform(glm::scale(glm::vec3(planet_size, planet_size, planet_size)));
  planet->SetTexture(0, planet_texture, true);
  planet->UpdateGPU();
  planet->SetBoundingSphereRadius(planet_size);
  SetMesh(planet);
  auto f = AssetLoader->LoadImage("White");
  assert(f);
  assert(f != planet->GetTexture());
}


ObjectPlanet::~ObjectPlanet()
{
  delete GetMesh();
}


void ObjectPlanet::AddPlanetRing(float start, float end)
{
  Mesh * ring = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
  ring->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));
  ring->SetTexture(0, AssetLoader->LoadImage("8k_saturn_ring_alpha"));

  const auto PI = 4.0f * std::atan(1.0f);
  const auto step = 2.0f * PI / 128.0f;
  
  unsigned int ind = 0;
  for(auto arc = -PI; arc < PI - step; arc += step)
    {
      std::vector<glm::vec3> vertices
        {
          glm::vec3(end   * std::sin(arc + step), end   * std::cos(arc + step), 0),
          glm::vec3(start * std::sin(arc + step), start * std::cos(arc + step), 0),
          glm::vec3(start * std::sin(arc),        start * std::cos(arc),        0),
          glm::vec3(end   * std::sin(arc),        end   * std::cos(arc),        0),
        };
      for(auto v : vertices)
        ring->AddVertex(v);

      std::vector<glm::vec2> texcoords
        {
          glm::vec2(1, 1),
          glm::vec2(0, 0),
          glm::vec2(0, 1),
          glm::vec2(1, 0),
        };
      for(auto t : texcoords)
        ring->AddTexCoord(t);

      std::vector<unsigned int> indices
        {
          0, 3, 1,
          1, 3, 2,
        };
      for(auto i : indices)
        ring->AddElement(ind + i);
      ind += 4;
    }
  ring->UpdateGPU();

  GetMesh()->AddChild(ring);
}

