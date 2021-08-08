/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Explosion.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include <iostream>


Explosion::Explosion(std::minstd_rand & random)
  : _time(999)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_BLEND | Mesh::OPTION_GENERIC_VEC3_INPUT);

  auto rand = [&random]() { return static_cast<double>(random() - random.min()) / static_cast<double>(random.max()); };

  const int rounds = 10;
  _fragment_count = rounds * (10 + static_cast<int>(40.0 * rand()));
  for(int i = 0; i < _fragment_count / rounds; i++)
    for(int j = 1; j <= rounds; j++)
      {
        float f = static_cast<float>(j) / static_cast<float>(rounds);
        _mesh->AddGenericVecInput(glm::normalize(glm::vec3(rand() - 0.5, rand() - 0.5, rand() - 0.5)) * f);
        _mesh->AddColor(glm::vec4(rand(), rand(), rand(), 1.0));
      }
  _mesh->SetPrimitiveType(GL_POINTS);
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Explosion"));
}


void Explosion::Activate(const glm::vec3 & position, const glm::vec3 & velocity)
{
  _time = 0;
  _mesh->ClearVertices();
  for(int i = 0; i < _fragment_count; i++)
    _mesh->AddVertex({0, 0, 0});
  Graphics->QueueUpdateGPU(_mesh);

  _position = position;
  _velocity = velocity;
  _model = glm::translate(_position);
}


void Explosion::Tick(double deltatime)
{
  _time += deltatime;
}


bool Explosion::IsAlive() const
{
  return _time < 2.0;
}


void Explosion::Draw(const Camera & camera) const
{
  if(!camera.IsInView(_position, 5))
    return;
  
  _mesh->GetShaderProgram()->Use();
  _mesh->GetShaderProgram()->SetFloat("in_time", static_cast<float>(_time));
  _mesh->GetShaderProgram()->SetVec("in_velocity", _velocity);
  _mesh->DrawSameShader(_model, camera.GetViewProjection() * _model);
}


void Explosion::Translate(const glm::vec3 & translation)
{
  _position += translation;
  _model = glm::translate(_position);
}
