#ifndef TEXTURE_RENDERER_HH_
#define TEXTURE_RENDERER_HH_

#include <GL/glew.h>


class TextureRenderer
{
public:
  TextureRenderer(unsigned int width, unsigned int height);
  ~TextureRenderer();

  void   BeginRender();
  void   EndRender();
  GLuint GetTextureId() const;
  
private:
  unsigned int _width;
  unsigned int _height;
  GLuint       _texture_id;
  GLuint       _framebuffer;
  GLuint       _depthbuffer;
};

#endif
