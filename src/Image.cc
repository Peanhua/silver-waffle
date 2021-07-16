/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Image.hh"
#include "glm.hh"
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include <SDL_image.h>


Image::Image(bool alpha, GLuint texture_id)
  : _width(0),
    _height(0),
    _bytes_per_pixel(0),
    _alpha(alpha),
    _data(nullptr),
    _texture_id(texture_id)
{
}


Image::Image(bool alpha)
  : Image(alpha, 0)
{
}


Image::Image(unsigned int width, unsigned int height, unsigned int bytes_per_pixel, bool alpha)
  : Image(alpha, 0)
{
  Resize(width, height, bytes_per_pixel);
}


Image::~Image()
{
  delete [] _data;
}


bool Image::Load(const std::string & filename)
{
  bool rv = false;
  
  auto bmp = IMG_Load(filename.c_str());
  if(bmp)
    {
      rv = Load(*bmp);
      SDL_FreeSurface(bmp);
    }

  return rv;
}


bool Image::Resize(unsigned int width, unsigned int height, unsigned int bytes_per_pixel)
{
  _width           = width;
  _height          = height;
  _bytes_per_pixel = bytes_per_pixel;

  assert(_bytes_per_pixel == 1 ||
         _bytes_per_pixel == 3 ||
         _bytes_per_pixel == 4);

  _data = new uint8_t[_width * _height * _bytes_per_pixel];
  return _data;
}


bool Image::Load(SDL_Surface & source)
{
  bool success = Resize(static_cast<unsigned int>(source.w), static_cast<unsigned int>(source.h), _alpha ? 4 : 3);
  if(success)
    {
      SDL_LockSurface(&source);

      for(int y = 0; y < source.h; y++)
        {
          uint8_t * src, * dst;

          src = static_cast<uint8_t *>(source.pixels) + y * source.pitch;
          dst = _data + (_height - static_cast<unsigned int>(y) - 1) * _width * _bytes_per_pixel;

          for(int x = 0; x < source.w; x++)
            {
              uint8_t r, g, b, a;

              a = 0xff;
              if(source.format->BytesPerPixel == 4)
                {
                  Uint32 p;
                  std::memcpy(&p, src, sizeof p);
                  SDL_GetRGBA(p, source.format, &r, &g, &b, &a);
                }
              else if(source.format->BytesPerPixel == 1)
                {
                  uint8_t colour_index;

                  colour_index = *src;
                  assert(colour_index < source.format->palette->ncolors);
                  r = source.format->palette->colors[colour_index].r;
                  g = source.format->palette->colors[colour_index].g;
                  b = source.format->palette->colors[colour_index].b;
                  a = 0xff;
                }
              else if(source.format->BytesPerPixel == 3)
                {
                  uint32_t value;

                  std::memset(&value, 0xff, sizeof value);
                  std::memcpy(&value, src, source.format->BytesPerPixel);
                  SDL_GetRGB(value, source.format, &r, &g, &b);
                  a = 0xff;
                }

              Uint32 ckey;

              if(SDL_GetColorKey(&source, &ckey) == 0)
                {
                  Uint32 rgb;
                  
                  rgb = SDL_MapRGB(source.format, r, g, b);
                  if(rgb == ckey)
                    a = 0x00;
                }

              dst[0] = r;
              dst[1] = g;
              dst[2] = b;
              if(_alpha)
                dst[3] = a;

              src += source.format->BytesPerPixel;
              dst += _bytes_per_pixel;
            }
        }
      

      SDL_UnlockSurface(&source);
    }

  assert(success);
  
  return success;
}



void Image::UpdateGPU(bool mipmapping, bool linear_filtering)
{
  if(_texture_id == 0)
    glGenTextures(1, &_texture_id);
  assert(_texture_id != 0);
  
  GLint internalformat;
  GLenum format;

  assert((_width * _bytes_per_pixel) % 4 == 0);

  glBindTexture(GL_TEXTURE_2D, _texture_id);

  if(_bytes_per_pixel == 1)
    {
      internalformat = GL_RED;
      format = GL_RED;
    }
  else if(_alpha)
    {
      internalformat = GL_RGBA8;
      format = GL_RGBA;
    }
  else
    {
      internalformat = GL_RGB8;
      format = GL_RGB;
    }
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  if(linear_filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if(mipmapping)
    {
      if(linear_filtering)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

      glTexImage2D(GL_TEXTURE_2D, 0, internalformat, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 0, format, GL_UNSIGNED_BYTE, _data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  else
    {
      if(linear_filtering == true)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, internalformat, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 0, format, GL_UNSIGNED_BYTE, _data);
    }
}


GLuint Image::GetTextureId() const
{
  return _texture_id;
}


void Image::SetTextureId(GLuint texture_id)
{
  _texture_id = texture_id;
}

 
void Image::ToSingleChannel(unsigned int channel)
{
  assert(channel < _bytes_per_pixel);
  assert(_width > 0);
  assert(_height > 0);

  std::vector<uint8_t> tmp;
  tmp.resize(_width * _height);
  
  for(unsigned int y = 0; y < _height; y++)
    for(unsigned int x = 0; x < _width; x++)
      tmp[x + y * _width] = _data[(x + y * _width) * _bytes_per_pixel + channel];

  _bytes_per_pixel = 1;
  _alpha           = false;

  for(unsigned int y = 0; y < _height; y++)
    for(unsigned int x = 0; x < _width; x++)
      _data[(x + y * _width) * _bytes_per_pixel] = tmp[x + y * _width];
}


void Image::CopyData(const uint8_t * src)
{
  std::memcpy(_data, src, _width * _height * _bytes_per_pixel);
}


unsigned int Image::GetWidth() const
{
  return _width;
}


unsigned int Image::GetHeight() const
{
  return _height;
}


unsigned int Image::GetBytesPerPixel() const
{
  return _bytes_per_pixel;
}


void Image::Clear()
{
  assert(_width > 0 && _height > 0 && _bytes_per_pixel > 0);
  memset(_data, 0, _width * _height * _bytes_per_pixel);
}


void Image::Blit(unsigned int x, unsigned int y, const Image & src)
{
  Blit(x, y, src, 0, 0, src.GetWidth(), src.GetHeight());
}



void Image::Blit(unsigned int x, unsigned int y, const Image & src, unsigned int src_x, unsigned int src_y, unsigned int width, unsigned int height)
{
  assert(src.GetWidth()  >= src_x + width );
  assert(src.GetHeight() >= src_y + height);
  assert(_width  >= x + width );
  assert(_height >= y + height);

  assert(src.GetBytesPerPixel() == GetBytesPerPixel());

  auto srcdata = src.GetData();
  for(unsigned int yi = 0; yi < height; yi++)
    memcpy(_data   + (x +     (yi + y)     * GetWidth())     * _bytes_per_pixel,
           srcdata + (src_x + (yi + src_y) * src.GetWidth()) * _bytes_per_pixel,
           width * _bytes_per_pixel);
}


uint8_t * Image::GetData() const
{
  return _data;
}


bool Image::Expand(unsigned int new_width, unsigned int new_height)
{
  bool rv = false;

  assert(_width <= new_width);
  assert(_height <= new_height);

  auto bpp = _bytes_per_pixel;

  auto newdata = new uint8_t[new_width * new_height * bpp];
  assert(newdata != NULL);
  if(newdata != NULL)
    {
      size_t slinelen, dlinelen;
      
      slinelen = _width    * bpp;
      dlinelen = new_width * bpp;

      std::memset(newdata, 0, new_width * new_height * bpp);

      for(unsigned int y = 0; y < _height; y++)
        std::memcpy(newdata + y * dlinelen, _data + y * slinelen, slinelen);
      
      _width  = new_width;
      _height = new_height;
      
      delete [] _data;
      _data = newdata;

      rv = true;
    }

  return rv;
}


void Image::ToSignedDistanceField(double shrink_min, double shrink_max)
{
  std::vector<double> distances;
  distances.resize(_width * _height);

  assert(_bytes_per_pixel > 0);
  auto channel = _bytes_per_pixel - 1;

  double max = 0;
  double min = 0;
  /* Calculate the distances for each x,y position and store them in 'distances'. */
  for(unsigned int y = 0; y < _height; y++)
    for(unsigned int x = 0; x < _width; x++)
      {
        glm::dvec2 curpos(x, y);
        glm::dvec2 closest(-1, -1);
        double     closest_distance = glm::distance(glm::dvec2(0, 0), glm::dvec2(_width, _height));
        bool       found = false;
        bool       isout = _data[(x + y * _width) * _bytes_per_pixel + channel] == 0;
        
        for(unsigned int y2 = 0; y2 < _height; y2++)
          for(unsigned int x2 = 0; x2 < _width; x2++)
            if(y2 != y || x2 != x)
              {
                unsigned int pos = (x2 + y2 * _width) * _bytes_per_pixel + channel;
                if((isout && _data[pos]) || (!isout && !_data[pos]))
                  {
                    double distance = glm::distance(curpos, glm::dvec2(x2, y2));
                    if(distance < closest_distance)
                      {
                        closest_distance = distance;
                        closest = glm::dvec2(x2, y2);
                        found = true;
                      }
                  }
              }
        assert(found);

        if(isout)
          closest_distance = -closest_distance;

        distances[x + y * _width] = closest_distance;

        if(found)
          {
            if(closest_distance < min)
              min = closest_distance;
            if(closest_distance > max)
              max = closest_distance;
          }
      }

  /* Map distances to 0..1, store the distance to all channels (rgba) into the image data (in range 0...255). */
  min *= (1.0 - shrink_min);
  max *= (1.0 - shrink_max);
  for(unsigned int y = 0; y < _height; y++)
    for(unsigned int x = 0; x < _width; x++)
      {
        double value;

        if(distances[x + y * _width] < 0.0)
          value = 0.5 * glm::smoothstep(min, 0.0, distances[x + y * _width]);
        else
          value = 0.5 * glm::smoothstep(0.0, max, distances[x + y * _width]) + 0.5;

        assert(value >= 0.0);
        assert(value <= 1.0);
        value *= 255.0;

        _data[(x + y * _width) * _bytes_per_pixel + channel] = static_cast<uint8_t>(value);
      }
}


glm::vec4 Image::GetRGBA(unsigned int x, unsigned int y) const
{
  assert(x < _width);
  assert(y < _height);
  glm::vec4 rgba(0, 0, 0, 0);

  auto ind = (x + y * _width) * _bytes_per_pixel;
  for(unsigned int i = 0; i < 4 && i < _bytes_per_pixel; i++)
    rgba[static_cast<int>(i)] = static_cast<float>(_data[ind + i]) / 255.0f;
  if(_bytes_per_pixel < 4)
    rgba.a = glm::length(rgba.rgb()) > 0.0f ? 1 : 0;
  
  return rgba;
}
