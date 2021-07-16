#ifndef GAUSSIAN_BLUR_HH_
#define GAUSSIAN_BLUR_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

class Mesh;

#include <vector>
#include <GL/glew.h>


class GaussianBlur
{
public:
  GaussianBlur();
  ~GaussianBlur();

  GLuint Blur(unsigned int count, GLuint source_texture_id);

private:
  std::vector<GLuint> _fbos;
  std::vector<GLuint> _texture_ids;
  std::vector<Mesh *> _meshes;
};

#endif
