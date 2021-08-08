/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectPlanetAtmosphere.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"


ObjectPlanetAtmosphere::ObjectPlanetAtmosphere(Scene * scene, const glm::vec2 & size, const std::vector<glm::vec3> & colors)
  : Object(scene, 0)
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
    std::vector<unsigned int> colinds {
      1,
      0,
      0,
      1
    };
    std::vector<unsigned int> indices {
      0, 3, 1,
      1, 3, 2
    };

    assert(colors.size() > 1);
    float zsize = size.y / static_cast<float>(colors.size() - 1);
    float z = (zsize - size.y) * 0.5f;
    for(unsigned int i = 0; i < colors.size() - 1; i++)
      {
        for(auto v : vertices)
          mesh->AddVertex(glm::vec3(0, 0, z) + v * glm::vec3(size.x * 0.5f, 1, 0.5f * zsize));
        for(auto ci : colinds)
          mesh->AddColor(colors[i + ci]);
        for(auto ind : indices)
          mesh->AddElement(i * 4 + ind);
        z += zsize;
      }
    Graphics->QueueUpdateGPU(mesh);
    mesh->CalculateBoundingVolumes();
    SetMesh(mesh);
  }

  SetCollisionChannels(0);
  SetCollidesWithChannels(0);
  SetUseHealth(false);
}
