#include "Image.hh"
#include <cassert>
#include <cstring>
#include <iostream>
#include <SDL_image.h>


Image::Image(bool alpha)
  : _width(0),
    _height(0),
    _bytes_per_pixel(0),
    _alpha(alpha),
    _data(nullptr),
    _texture_id(0)
{
}


Image::~Image()
{
  delete [] _data;
}


bool Image::Load(const std::string & filename)
{
  auto bmp = IMG_Load(filename.c_str());
  if(bmp)
    {
      return Load(*bmp);
    }
  else
    {
      std::cout << "Failed to load '" << filename << "': " << SDL_GetError() << std::endl;
      return false;
    }
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
      internalformat = GL_ALPHA8;
      format = GL_ALPHA;
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
