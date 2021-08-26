#ifndef IMAGE_HH_
#define IMAGE_HH_
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
#include <string>
#include <cstdint>
#include <SDL.h>
#include <GL/glew.h>


class Image
{
public:
  Image(bool alpha);
  Image(bool alpha, GLuint texture_id);
  Image(unsigned int width, unsigned int height, unsigned int bytes_per_pixel, bool alpha, bool mipmapping);
  ~Image();

  bool   Load(const std::string & filename);
  bool   Load(SDL_Surface & source);
  bool   Save(const std::string & filename);
  void   UpdateGPU();
  GLuint GetTextureId() const;
  void   SetTextureId(GLuint texture_id);
  bool   Expand(unsigned int new_width, unsigned int new_height);
  void   ToSingleChannel(unsigned int channel); // Remove all but the specified channel.
  void   ToSignedDistanceField(double shrink_min, double shrink_max); // Higher shrink value means more distance values are set to zero (min) or to 1 (max).
  void   BlackToAlpha();
  void   Blit(unsigned int x, unsigned int y, const Image & src);
  void   Blit(unsigned int x, unsigned int y, const Image & src, unsigned int src_x, unsigned int src_y, unsigned int width, unsigned int height);
  void   CopyData(const uint8_t * src);
  void   Clear();

  unsigned int GetWidth()         const;
  unsigned int GetHeight()        const;
  unsigned int GetBytesPerPixel() const;
  uint8_t *    GetData()          const;
  glm::vec4    GetRGBA(unsigned int x, unsigned int y) const;
  
private:
  unsigned int         _width;
  unsigned int         _height;
  unsigned int         _bytes_per_pixel;
  bool                 _alpha;
  bool                 _mipmapping;
  bool                 _linear_filtering;
  uint8_t *            _data;
  
  GLuint               _texture_id;

  bool Resize(unsigned int width, unsigned int height, unsigned int bytes_per_pixel);
};

#endif
