/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "AdditiveBlending.hh"
#include "Image.hh"
#include "MeshOverlay.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"


AdditiveBlending::AdditiveBlending()
{
  _mesh = new MeshOverlay();
  _mesh->SetTexture(0, new Image(false));
  _mesh->SetTexture(1, new Image(false));
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("AdditiveBlending"));
}


void AdditiveBlending::Blend(GLuint a, GLuint b)
{
  _mesh->GetTexture(0)->SetTextureId(a);
  _mesh->GetTexture(1)->SetTextureId(b);
  _mesh->Draw(glm::mat4(1), glm::mat4(1));
}

