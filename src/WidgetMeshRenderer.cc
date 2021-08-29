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
#include "SubsystemGfx.hh"
#include "TextureRenderer.hh"
#include "UniformBufferObject.hh"


WidgetMeshRenderer::WidgetMeshRenderer(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : WidgetMeshRenderer(parent, position, size, nullptr, glm::mat4(1), glm::mat4(1))
{
}


WidgetMeshRenderer::WidgetMeshRenderer(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, Mesh * mesh, const glm::mat4 & model, const glm::mat4 & view)
  : Widget(parent, position, size),
    _draw_empty(true),
    _mesh(mesh),
    _model(model),
    _view(view)
{
  _texture_renderer = new TextureRenderer(512, 512);
  SetImage(new Image(true));
  Graphics->QueueUpdateGPU(this);
}


WidgetMeshRenderer::~WidgetMeshRenderer()
{
  delete _texture_renderer;
}


void WidgetMeshRenderer::SetMesh(Mesh * mesh)
{
  _mesh = mesh;
}


void WidgetMeshRenderer::SetModel(const glm::mat4 & model)
{
  _model = model;
}


void WidgetMeshRenderer::SetView(const glm::mat4 & view)
{
  _view = view;
}


void WidgetMeshRenderer::Render()
{
  if(!_mesh)
    return;

  glEnable(GL_DEPTH_TEST);
  _texture_renderer->BeginRender();
  {
    glm::mat4 proj = glm::perspective(glm::radians(60.0), 512.0 / 512.0, 0.001, 100.0);

    auto ubo = UniformBufferObject::GetUniformBufferObject();
    ubo->SetVec(   "in_glow",        glm::vec3(0, 0, 0));
    ubo->SetVec(   "in_colormod",    glm::vec4(1, 1, 1, 1));
    ubo->SetMatrix("in_view",        _view);
    ubo->SetMatrix("in_projection",  proj);
    ubo->SetFloat( "in_time",        0);
    ubo->SetVec(   "in_light_color", glm::vec3(1, 1, 1));
    ubo->Flush();
    _mesh->Draw(_model, proj * _view * _model);
  }
  _texture_renderer->EndRender();
  GetImage()->SetTextureId(_texture_renderer->GetTextureId());
}


void WidgetMeshRenderer::SetMultiRender(const std::vector<glm::ivec2> & offsets)
{
  _multioffsets = offsets;
}


void WidgetMeshRenderer::Draw() const
{
  if(!GetIsVisible())
    return;

  auto image = GetImageMesh();
  if(image)
    {
      if(_multioffsets.size() == 0)
        {
          if(_draw_empty)
            image->Draw(glm::mat4(1), GetMVP());
        }
      else
        for(auto offset : _multioffsets)
          {
            auto model = glm::translate(glm::mat4(1), glm::vec3(offset, 0));
            image->Draw(model, GetMVP() * model);
          }
    }
}
