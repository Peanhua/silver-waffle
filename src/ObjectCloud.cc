/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectCloud.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#include <color/color.hpp>
#pragma GCC diagnostic pop


ObjectCloud::ObjectCloud(Scene * scene, unsigned int random_seed)
  : ObjectMovable(scene, random_seed)
{
  SetIsAffectedByGravity(false);

  int meshtype = GetRand() < 0.67f ? 1 : 2;
  auto cmesh = AssetLoader->LoadMesh("Cloud" + std::to_string(meshtype));
  SetMesh(new Mesh(0));
  GetMesh()->AddChild(cmesh);
  GetMesh()->SetShaderProgram(cmesh->GetShaderProgram());
  glm::vec3 scale {
    5.0f + 5.0f * GetRand(),
    5.0f + 5.0f * GetRand(),
    5.0f + 5.0f * GetRand()
  };
  float angle = (GetRand() - 0.5f) * 45.0f;
  if(GetRand() < 0.5f)
    angle += 180.0f;
  glm::mat4 t = glm::toMat4(glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1)));
  t = glm::scale(t, scale);
  GetMesh()->ApplyTransform(t);
  
  auto cursize = cmesh->GetBoundingBoxHalfSize();
  cursize *= scale;
  GetMesh()->SetBoundingSphereRadius(glm::length(cursize));

  SetCollisionChannels(0);
  SetCollidesWithChannels(0);
  SetUseHealth(false);
  AddImpulse({10.0f * GetRand(), 0, 0});
}


void ObjectCloud::SetColorFromImage(Image * image)
{
  assert(image);
  auto color = image->GetRGBA({GetRand(), GetRand()}).rgb();
  color::rgb<float> rgb({color.r, color.g, color.b});
  color::hsv<float> hsv(rgb);
  hsv[1] = std::clamp(hsv[1] + 50.0f, 0.0f, 100.0f); // saturation
  hsv[2] = std::clamp(hsv[2] + 25.0f, 0.0f, 100.0f); // value
  rgb = hsv;
  SetColor({rgb[0], rgb[1], rgb[2]});
}
