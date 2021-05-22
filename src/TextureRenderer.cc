#include "TextureRenderer.hh"
#include <cassert>


TextureRenderer::TextureRenderer(unsigned int width, unsigned int height)
  : _width(width),
    _height(height)
{
  glGenFramebuffers(1, &_framebuffer);
  assert(_framebuffer != 0);
  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

  glGenTextures(1, &_texture_id);
  assert(_texture_id != 0);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenRenderbuffers(1, &_depthbuffer);
  assert(_depthbuffer != 0);
  glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture_id, 0);
  GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, buffers);

  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


TextureRenderer::~TextureRenderer()
{
}


void TextureRenderer::BeginRender()
{
  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
  glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void TextureRenderer::EndRender()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 1024, 768);
}


GLuint TextureRenderer::GetTextureId() const
{
  return _texture_id;
}
