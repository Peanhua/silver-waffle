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
  auto nums = AssetLoader->LoadMesh("Numbers");
  std::vector<Mesh *> numbers;
  for(char i = '0'; i <= '9'; i++)
    {
      auto n = nums->FindChild(std::string("Number") + i);
      assert(n);
      numbers.push_back(n);
    }
  
  
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      _drums.push_back(CreateDrum(numbers));
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

  SetScore(0);
}


bool ScoreReel::Tick(double deltatime)
{
  bool should_draw = false;
  for(unsigned int i = 0; i < _drums.size(); i++)
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
  ShaderProgram::SetUBOVec3("Data", "in_light_color", glm::vec3(1.00, 0.59, 0.19));

  glm::mat4 proj = glm::perspective(glm::radians(30.0), 512.0 / 128.0, 0.001, 100.0);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -static_cast<float>(_drum_count) * static_cast<float>(_drum_width) * 0.65f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.5f * -static_cast<float>(_drum_count) * static_cast<float>(_drum_width), 0, 0));

  ShaderProgram::SetUBOMatrix("Data", "in_view",       view);
  ShaderProgram::SetUBOMatrix("Data", "in_projection", proj);

  _background->Draw(model, proj * view * model);
  
  for(unsigned int i = 0; i < _drums.size(); i++)
    {
      glm::mat4 drummodel(model);
      drummodel = glm::translate(drummodel, glm::vec3(static_cast<float>(_drum_width) * (static_cast<float>(i) + 0.125f), 0, 0));
      drummodel = glm::rotate(drummodel, glm::radians(180.0f), glm::vec3(1, 0, 0));
      drummodel = glm::rotate(drummodel, glm::radians(static_cast<float>(_drums_angles[i])), glm::vec3(1, 0, 0));

      _drums[i]->Draw(drummodel, proj * view * drummodel);
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
      const double adjustment = 0.47;
      _drums_target_angles[drumind] = 360.0 / 10.0 * (adjustment + static_cast<double>(10 - drumscore));
    }
}


unsigned int ScoreReel::GetScore() const
{
  return _score;
}


Mesh * ScoreReel::CreateDrum(const std::vector<Mesh *> numbers) const
{
  Mesh * drum = new Mesh(0);
  
  glm::mat4 rot(1);
  for(unsigned int i = 0; i <= 9; i++)
    {
      auto num = new Mesh(*numbers[i]);
      auto forward = glm::column(rot, 1);
      auto mov = glm::translate(glm::mat4(1), forward.xyz() * 1.3f);
      num->ApplyTransform(mov * glm::rotate(rot, glm::radians(-18.0f), glm::vec3(1, 0, 0)));
      drum->AddChild(num);
      rot = glm::rotate(rot, glm::radians(36.0f), glm::vec3(1, 0, 0));
    }
  
  drum->UpdateGPU();

  return drum;
}

