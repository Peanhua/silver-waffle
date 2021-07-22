/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ScoreReel.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include <cassert>

//#define LARGE


ScoreReel::ScoreReel(unsigned int drum_count)
  : _drum_count(drum_count),
    _drum_width(0.75)
{
  _drum = AssetLoader->LoadMesh("ScoreReelDrum");
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      _drums_angles.push_back(0);
      _drums_target_angles.push_back(0);
    }

  
  _background = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_BLEND);
  _background->SetShaderProgram(AssetLoader->LoadShaderProgram("Mask"));
  
  std::vector<glm::vec3> vertices {
    glm::vec3(_drum_count * _drum_width,  0,  1.2), // 0
    glm::vec3(_drum_count * _drum_width,  0, -1.2), // 1
    glm::vec3(0.5 - _drum_width,          0, -1.2), // 2
    glm::vec3(0.5 - _drum_width,          0,  1.2), // 3
#ifndef LARGE
    glm::vec3(0.5 - _drum_width,          0,  0.0), // 4
    glm::vec3(_drum_count * _drum_width,  0,  0.0), // 5
    glm::vec3(0.5 - _drum_width,         -5,  1.5), // 6
    glm::vec3(_drum_count * _drum_width, -5,  1.5), // 7
    glm::vec3(0.5 - _drum_width,         -5, -1.5), // 8
    glm::vec3(_drum_count * _drum_width, -5, -1.5), // 9
#endif
  };
  
  std::vector<unsigned int> indices {
#ifdef LARGE
    // Center:
    0, 3, 1,
    1, 3, 2,
#else
    // Top:
    5, 6, 4,
    6, 5, 7,
    // Bottom:
    5, 4, 8,
    8, 9, 5,
#endif
  };
  
  for(auto v : vertices)
    _background->AddVertex(v);
  
  for(auto ii : indices)
    _background->AddElement(ii);
  
  _background->UpdateGPU();

  _projection = glm::perspective(glm::radians(30.0), 512.0 / 128.0, 0.001, 100.0);
  _view = glm::lookAt(glm::vec3(0.0f, -3.5, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  _viewprojection = _projection * _view;
  _model = glm::translate(glm::mat4(1), glm::vec3(0.5f * -static_cast<float>(_drum_count) * static_cast<float>(_drum_width), 0, 0));
  
  SetScore(0);
}


bool ScoreReel::Tick(double deltatime)
{
  bool should_draw = false;
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      auto diff = _drums_target_angles[i] - _drums_angles[i];
      _drums_angles[i] += diff * deltatime;
      if(std::abs(diff) > 1.0)
        should_draw = true;
    }
  return should_draw;
}


void ScoreReel::Draw() const
{
  auto shader = AssetLoader->LoadShaderProgram("SceneObject-Color");
  assert(shader);
  shader->Use();
  shader->SetVec("in_glow", glm::vec3(0.0, 0.0, 0.0));
  shader->SetVec("in_colormod", glm::vec3(1, 1, 1));
  ShaderProgram::SetUBOVec3("Data",   "in_light_color", glm::vec3(1.00, 0.59, 0.19));
  ShaderProgram::SetUBOMatrix("Data", "in_view",        _view);
  ShaderProgram::SetUBOMatrix("Data", "in_projection",  _projection);

  _background->Draw(_model, _viewprojection * _model);
  
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      glm::mat4 drummodel(_model);
      drummodel = glm::translate(drummodel, glm::vec3(static_cast<float>(_drum_width) * (static_cast<float>(i) + 0.125f), 0, 0));
      drummodel = glm::rotate(drummodel, glm::radians(static_cast<float>(_drums_angles[i])), glm::vec3(1, 0, 0));
      _drum->Draw(drummodel, _viewprojection * drummodel);
    }
}


void ScoreReel::SetScore(unsigned int score)
{
  _score = score;
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      const auto drumscore = score % 10;
      score /= 10;
      
      const auto drumind = _drum_count - i - 1;
      _drums_target_angles[drumind] = 360.0 / 10.0 * static_cast<double>(10 - drumscore);
    }
}


unsigned int ScoreReel::GetScore() const
{
  return _score;
}
