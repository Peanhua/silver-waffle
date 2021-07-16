#ifndef MESH_PROGRESSBAR_HH_
#define MESH_PROGRESSBAR_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Mesh.hh"

class MeshProgressBar : public Mesh
{
public:
  MeshProgressBar(float width, float height, const glm::vec3 & color);

  void  SetColor(const glm::vec3 & color);
  void  SetValue(float value);
  float GetValue() const;

  void  PreDrawSetupShader(ShaderProgram * shader_program) const override;
  
private:
  float     _width;
  float     _height;
  glm::vec3 _color;
  float     _value;
};

#endif
