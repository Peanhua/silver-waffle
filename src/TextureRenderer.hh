#ifndef TEXTURE_RENDERER_HH_
#define TEXTURE_RENDERER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <GL/glew.h>
#include <vector>


class TextureRenderer
{
public:
  TextureRenderer(unsigned int width, unsigned int height, unsigned int output_buffer_count = 1);
  ~TextureRenderer();

  void   BeginRender();
  void   EndRender();
  GLuint GetTextureId(unsigned int index = 0) const;
  
private:
  unsigned int _width;
  unsigned int _height;
  std::vector<GLuint> _texture_ids;
  GLuint       _framebuffer;
  GLuint       _depthbuffer;
};

#endif
