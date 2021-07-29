/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetPlayerShip.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "TextureRenderer.hh"


WidgetPlayerShip::WidgetPlayerShip(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size)
{
  _mesh = AssetLoader->LoadMesh("Player");
  _texture_renderer = new TextureRenderer(512, 512);
  SetImage(new Image(true));
  Render();
}


WidgetPlayerShip::~WidgetPlayerShip()
{
  delete _texture_renderer;
}


void WidgetPlayerShip::Render()
{
  glEnable(GL_DEPTH_TEST);
  _texture_renderer->BeginRender();
  {
    glm::mat4 proj = glm::perspective(glm::radians(60.0), 512.0 / 512.0, 0.001, 1000.0);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    glm::mat4 model = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.1, -0.4, 0));

    ShaderProgram::SetUBOVec3("Data",   "in_glow",        glm::vec3(0, 0, 0));
    ShaderProgram::SetUBOVec3("Data",   "in_colormod",    glm::vec3(1, 1, 1));
    ShaderProgram::SetUBOMatrix("Data", "in_view",        view);
    ShaderProgram::SetUBOMatrix("Data", "in_projection",  proj);
    ShaderProgram::SetUBOVec3("Data",   "in_light_color", glm::vec3(1, 1, 1));

    _mesh->Draw(model, proj * view * model);
  }
  _texture_renderer->EndRender();
  GetImage()->SetTextureId(_texture_renderer->GetTextureId());
}
