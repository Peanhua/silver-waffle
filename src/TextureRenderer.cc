/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "TextureRenderer.hh"
#include "SubsystemGfx.hh"
#include "SubsystemSettings.hh"
#include <cassert>


TextureRenderer::TextureRenderer(unsigned int width, unsigned int height, unsigned int output_buffer_count)
  : _width(width),
    _height(height),
    _buffer_count(output_buffer_count)
{
  Graphics->QueueUpdateGPU(this);
}


void TextureRenderer::UpdateGPU()
{
  glGenFramebuffers(1, &_framebuffer);
  assert(_framebuffer != 0);
  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

  glGenRenderbuffers(1, &_depthbuffer);
  assert(_depthbuffer != 0);
  glBindRenderbuffer(GL_RENDERBUFFER, _depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer);

  for(unsigned int i = 0; i < _buffer_count; i++)
    {
      _texture_ids.push_back(0);
      glGenTextures(1, &_texture_ids[i]);
      assert(_texture_ids[i] != 0);
      glBindTexture(GL_TEXTURE_2D, _texture_ids[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, _texture_ids[i], 0);
    }

  std::vector<GLenum> buffers;
  for(unsigned int i = 0; i < _buffer_count; i++)
    buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
  glDrawBuffers(static_cast<GLsizei>(_buffer_count), buffers.data());

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
  glViewport(0, 0, Settings->GetInt("screen_width"), Settings->GetInt("screen_height"));
}


GLuint TextureRenderer::GetTextureId(unsigned int index) const
{
  assert(index < _texture_ids.size());
  return _texture_ids[index];
}
