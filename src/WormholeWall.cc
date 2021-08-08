/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WormholeWall.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include <cassert>


WormholeWall::WormholeWall(double length, double height)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT | Mesh::OPTION_BLEND);
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
  
  std::vector<glm::vec3> vertices {
    glm::vec3(length, 0, height), // 0
    glm::vec3(length, 0,      0), // 1
    glm::vec3(0,      0,      0), // 2
    glm::vec3(0,      0, height), // 3
  };
  
  std::vector<unsigned int> indices {
    0, 3, 1,
    1, 3, 2,
  };
  std::vector<glm::vec4> colors {
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.0f),
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.0f),
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.5f),
    glm::vec4(glm::vec3(0.64, 0.12, 0.77) * 0.1f, 0.5f),
  };
  
  for(auto v : vertices)
    _mesh->AddVertex(v);
  for(auto c : colors)
    _mesh->AddColor(c);
  for(auto i : indices)
    _mesh->AddElement(i);

  Graphics->QueueUpdateGPU(_mesh);
}


void WormholeWall::Tick(double deltatime)
{
  assert(deltatime == deltatime);
}


void WormholeWall::Draw(const glm::mat4 & model, const glm::mat4 & mvp) const
{
  _mesh->Draw(model, mvp);
}
