/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectPlanetGround.hh"
#include "CollisionShapeOBB.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"


ObjectPlanetGround::ObjectPlanetGround(Scene * scene, const glm::vec2 & size, Image * texture_map)
  : Object(scene, 0)
{
  AddToCollisionChannel(Object::CollisionChannel::TERRAIN);

  {
    auto mesh = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_COLOR | Mesh::OPTION_TEXTURE | Mesh::OPTION_NORMAL);
    mesh->SetTexture(0, texture_map);
    mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("SceneObject-Texture"));

    std::vector<glm::vec3> vertices {
      glm::vec3( 1,  1, 1),
      glm::vec3( 1, -1, 1),
      glm::vec3(-1, -1, 1),
      glm::vec3(-1,  1, 1),
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
        mesh->AddVertex(v * glm::vec3(size, 1) * 0.5f);
        mesh->AddColor({1.0, 1.0, 1.0});
        mesh->AddNormal({0, 0, 1});
      }
    for(auto tc : texcoords)
      mesh->AddTexCoord(tc);
    for(auto i : indices)
      mesh->AddElement(i);
    
    Graphics->QueueUpdateGPU(mesh);
    mesh->CalculateBoundingVolumes();
    SetMesh(mesh);
  }

  SetUseHealth(false);

  SetCollisionShape(new CollisionShapeOBB(this, glm::vec3(size, 1) * 0.5f, {0.5f, 0.4f, 0.0f}));
}
