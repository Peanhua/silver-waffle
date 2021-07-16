/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "MeshOverlay.hh"
#include "Image.hh"

MeshOverlay::MeshOverlay()
  : Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_TEXTURE)
{
  SetTexture(0, new Image(false));

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
    AddVertex(v);
  for(auto tc : texcoords)
    AddTexCoord(tc);
  for(auto i : indices)
    AddElement(i);
    
  UpdateGPU();
}
