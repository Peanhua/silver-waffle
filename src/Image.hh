#ifndef IMAGE_HH_
#define IMAGE_HH_

#include <string>
#include <cstdint>
#include <SDL.h>
#include <GL/glew.h>


class Image
{
public:
  Image(bool alpha);
  ~Image();

  bool   Load(const std::string & filename);
  bool   Load(SDL_Surface & source);
  void   UpdateGPU(bool mipmapping, bool linear_filtering);
  GLuint GetTextureId() const;
  
private:
  unsigned int         _width;
  unsigned int         _height;
  unsigned int         _bytes_per_pixel;
  bool                 _alpha;
  uint8_t *            _data;
  
  GLuint               _texture_id;

  bool Resize(unsigned int width, unsigned int height, unsigned int bytes_per_pixel);
};

#endif
