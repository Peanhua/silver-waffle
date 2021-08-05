/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetMeshRenderer.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "TextureRenderer.hh"
#include "UniformBufferObject.hh"


WidgetMeshRenderer::WidgetMeshRenderer(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, Mesh * mesh, const glm::mat4 & model, const glm::mat4 & view)
  : Widget(parent, position, size),
    _mesh(mesh),
    _model(model),
    _view(view)
{
  _texture_renderer = new TextureRenderer(512, 512);
  SetImage(new Image(true));
  Render();
}


WidgetMeshRenderer::~WidgetMeshRenderer()
{
  delete _texture_renderer;
}


void WidgetMeshRenderer::Render()
{
  glEnable(GL_DEPTH_TEST);
  _texture_renderer->BeginRender();
  {
    glm::mat4 proj = glm::perspective(glm::radians(60.0), 512.0 / 512.0, 0.001, 100.0);

    UniformBufferObject::GetUniformBufferObject()->SetVec(   "in_glow",        glm::vec3(0, 0, 0));
    UniformBufferObject::GetUniformBufferObject()->SetVec(   "in_colormod",    glm::vec4(1, 1, 1, 1));
    UniformBufferObject::GetUniformBufferObject()->SetMatrix("in_view",        _view);
    UniformBufferObject::GetUniformBufferObject()->SetMatrix("in_projection",  proj);
    UniformBufferObject::GetUniformBufferObject()->SetVec(   "in_light_color", glm::vec3(1, 1, 1));

    _mesh->Draw(_model, proj * _view * _model);
  }
  _texture_renderer->EndRender();
  GetImage()->SetTextureId(_texture_renderer->GetTextureId());
}
