/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "GaussianBlur.hh"
#include "Image.hh"
#include "MeshOverlay.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"


GaussianBlur::GaussianBlur()
{
  std::vector<std::string> names
    {
      "Horizontal",
      "Vertical"
    };
  for(auto name : names)
    {
      GLuint fbo;
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      _fbos.push_back(fbo);

      GLuint texture_id;
      glGenTextures(1, &texture_id);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Settings->GetInt("screen_width"), Settings->GetInt("screen_height"), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      _texture_ids.push_back(texture_id);
            
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

      assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

      auto mesh = new MeshOverlay();
      mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("GaussianBlur-" + name));
      _meshes.push_back(mesh);
    }
}


GaussianBlur::~GaussianBlur()
{
}


GLuint GaussianBlur::Blur(unsigned int count, GLuint source_texture_id)
{
  for(unsigned int i = 0; i < count * _fbos.size(); i++)
    {
      unsigned int ind = i % static_cast<unsigned int>(_fbos.size());

      glBindFramebuffer(GL_FRAMEBUFFER, _fbos[ind]);
      _meshes[ind]->GetTexture()->SetTextureId(source_texture_id);
      _meshes[ind]->Draw(glm::mat4(1), glm::mat4(1), glm::mat4(1), glm::mat4(1));

      source_texture_id = _texture_ids[ind];
    }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return _texture_ids[0];
}

