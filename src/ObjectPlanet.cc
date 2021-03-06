/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectPlanet.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include <cassert>
#include <numbers>


ObjectPlanet::ObjectPlanet(Scene * scene, SolarSystemObject * solar_system_object, Image * planet_texture, double planet_radius)
  : ObjectMovable(scene, 0),
    _solar_system_object(solar_system_object),
    _planet_radius(planet_radius),
    _ring_max(1)
{
  SetCollisionChannels(0);
  SetCollidesWithChannels(0);
  SetIsAffectedByGravity(false);
  auto planet = new Mesh(*AssetLoader->LoadMesh("Planet", "Generic"));
  assert(planet);
  planet->ApplyTransform(glm::scale(glm::vec3(planet_radius, planet_radius, planet_radius)));
  planet->SetTexture(0, planet_texture, true);
  Graphics->QueueUpdateGPU(planet);
  planet->SetBoundingSphereRadius(planet_radius);
  SetMesh(planet);
  SetTickingRequiresPlayerAlive(true);
}


ObjectPlanet::~ObjectPlanet()
{
  delete GetMesh();
}


void ObjectPlanet::AddPlanetRing(double start, double end)
{
  _ring_max = std::max(_ring_max, end);
  Mesh * ring = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
  ring->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));
  ring->SetTexture(0, AssetLoader->LoadImage("8k_saturn_ring_alpha"));

  const auto step = 2.0 * std::numbers::pi / 128.0;
  
  unsigned int ind = 0;
  for(auto arc = -std::numbers::pi; arc < std::numbers::pi - step; arc += step)
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
  Graphics->QueueUpdateGPU(ring);

  GetMesh()->AddChild(ring);
}


SolarSystemObject * ObjectPlanet::GetSolarSystemObject() const
{
  return _solar_system_object;
}


double ObjectPlanet::GetVisualBoundingSphereRadius() const
{
  return _planet_radius * _ring_max;
}
