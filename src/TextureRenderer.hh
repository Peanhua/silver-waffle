#ifndef TEXTURE_RENDERER_HH_
#define TEXTURE_RENDERER_HH_

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
