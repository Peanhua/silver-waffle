/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Component.hh"
#include "Mesh.hh"
#include "ObjectSpaceship.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "UniformBufferObject.hh"


Component::Component()
  : _visible(true)
{
}


Component::~Component()
{
}


void Component::Draw(const glm::mat4 & transform, const glm::mat4 & view_projection) const
{
  if(!_visible)
    return;

  auto ubo = UniformBufferObject::GetUniformBufferObject();
  ubo->SetVec("in_glow", _glow);
  ubo->SetVec("in_colormod", _color);
  ubo->Flush();
  
  auto model = glm::translate(transform, _position);
  _mesh->Draw(model, view_projection * model);
}


void Component::SetPosition(const glm::vec3 & position)
{
  _position = position;
}


void Component::SetMesh(Mesh * mesh)
{
  _mesh = mesh;
}


void Component::SetColor(const glm::vec4 & color)
{
  _color = color;
}


void Component::SetGlow(const glm::vec3 & glow)
{
  _glow = glow;
}


void Component::SetVisible(bool visible)
{
  _visible = visible;
}


void Component::Tick([[maybe_unused]] double deltatime)
{
}


ComponentEngineExhaustFX::ComponentEngineExhaustFX(ObjectSpaceship * spaceship, unsigned int engine_id)
  : Component(),
    _spaceship(spaceship),
    _engine_id(engine_id),
    _current(0)
{
  SetGlow({1.0f, 0.5f, 0.0f});
  SetColor({1.0f, 0.75f, 0.0f, 0.5f});
  SetMesh(AssetLoader->LoadMesh("EngineExhaust"));
  SetPosition({0, -0.7, 0});
}

void ComponentEngineExhaustFX::Tick([[maybe_unused]] double deltatime)
{
  double engine = _spaceship->GetEngineThrottle(_engine_id);
  if(engine > 0.99)
    _current = 0.7 * _current + 0.3 * engine;
  else
    _current = 0.8 * _current + 0.2 * engine;

  SetGlow({_current, _current * 0.75, _current * 0.25});
  SetColor({1.0f, 0.75f, 0.0f, _current});
  SetVisible(_current > 0.01);
}
