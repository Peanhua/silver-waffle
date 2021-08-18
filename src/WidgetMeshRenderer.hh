#ifndef WIDGET_MESH_RENDERER_HH_
#define WIDGET_MESH_RENDERER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"
#include "Widget.hh"
#include <vector>

class Mesh;
class TextureRenderer;


class WidgetMeshRenderer : public Widget
{
public:
  WidgetMeshRenderer(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, Mesh * mesh, const glm::mat4 & model, const glm::mat4 & view);
  ~WidgetMeshRenderer();

  void Render() override;
  void Draw() const override;
  void SetMultiRender(const std::vector<glm::ivec2> & offsets);
  
private:
  Mesh *            _mesh;
  glm::mat4         _model;
  glm::mat4         _view;
  TextureRenderer * _texture_renderer;
  std::vector<glm::ivec2> _multioffsets;
};

#endif
