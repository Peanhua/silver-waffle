/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SpaceParticles.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include "UniformBufferObject.hh"


SpaceParticles::SpaceParticles(double radius_min, double radius_max, unsigned long random_seed, float startpos, float endpos)
  : _radius_min(radius_min),
    _radius_max(radius_max),
    _startpos(startpos),
    _endpos(endpos),
    _time(0),
    _add_star_timer(0),
    _particle_length(0),
    _star_count(400),
    _next_index(0),
    _random(random_seed),
    _rdist(0, 1)
{
  _mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_BLEND | Mesh::OPTION_VERTEX_W | Mesh::OPTION_GENERIC_VEC2_INPUT | Mesh::OPTION_DYNAMIC_VERTEX);
  _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("SpaceJunk"));
  _mesh->SetPrimitiveType(GL_POINTS);
  const double min_speed = 5.0;
  const double max_speed = 30.0;
  for(unsigned int i = 0; i < _star_count; i++)
    {
      _mesh->AddVertex(glm::vec4(0, _endpos, 0, 0));
      _mesh->AddColor(glm::vec4(GetRandom(0.65), GetRandom(0.65), GetRandom(0.65), 1.0));

      double speed = min_speed + GetRandom() * (max_speed - min_speed);
      _mesh->AddGenericVecInput(glm::vec2(speed, GetRandom()));
    }
  Graphics->QueueUpdateGPU(_mesh);

  for(int i = 0; i < 60 * 10; i++)
    Tick(1.0 / 60.0);
}


SpaceParticles::~SpaceParticles()
{
  delete _mesh;
}


double SpaceParticles::GetRandom(double min)
{
  return min + (1.0 - min) * _rdist(_random);
}


void SpaceParticles::Tick(double deltatime)
{
  _time += deltatime;

  if(_add_star_timer < 0.0)
    {
      for(int i = 0; i < 10; i++)
        {
          const auto v = _mesh->GetVertex4(_next_index);
          const auto g = _mesh->GetGenericVec2(_next_index);
          auto pos = v.y - g.x * (static_cast<float>(_time) - v.w);
          if(pos < _endpos)
            {
              ResetStar(_next_index);
              _add_star_timer += 0.05;
              break;
            }
  
          _next_index++;
          if(_next_index >= _star_count)
            _next_index = 0;
        }
    }
  else
    _add_star_timer -= deltatime;
}


void SpaceParticles::SetMode(bool lines, float particle_length)
{
  if(lines)
    _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("SpaceJunkLines"));
  else
    _mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("SpaceJunk"));

  _particle_length = 20.0f * particle_length;
}


void SpaceParticles::ResetStar(unsigned int index)
{
  double angle = GetRandom() * glm::radians(360.0);
  auto pos = glm::vec3(_radius_min + GetRandom() * (_radius_max - _radius_min),
                       _startpos,
                       0);
  auto rot = glm::rotate(glm::mat4(1), static_cast<float>(angle), glm::vec3(0, 1, 0));
  pos = rot * glm::vec4(pos, 1);

  _mesh->SetVertex(index, glm::vec4(pos, _time));
  Graphics->QueueUpdateGPU(_mesh, index);
}


void SpaceParticles::Draw(const Camera & camera) const
{
  const glm::mat4 & mvp = camera.GetViewProjection();

  auto shader = _mesh->GetShaderProgram();
  shader->Use();
  shader->SetFloat("in_particle_length", _particle_length);
  shader->SetFloat("in_junktime", static_cast<float>(_time));
  _mesh->DrawSameShader(glm::mat4(1), mvp);
}
