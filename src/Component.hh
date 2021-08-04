#ifndef COMPONENT_HH_
#define COMPONENT_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "glm.hh"

class Mesh;
class ObjectSpaceship;


class Component
{
public:
  Component();
  virtual ~Component();

  virtual void Tick(double deltatime);
  void Draw(const glm::mat4 & transform, const glm::mat4 & view_projection) const;

  void SetPosition(const glm::vec3 & position);
  //  void SetOrientation(const glm::quat & orientation);
  void SetMesh(Mesh * mesh);
  void SetColor(const glm::vec4 & color);
  void SetGlow(const glm::vec3 & glow);

  void SetVisible(bool visible);
  
private:
  // Object * _owner;
  glm::vec3 _position;
  //  glm::quat _orientation;
  Mesh *    _mesh;
  glm::vec4 _color;
  glm::vec3 _glow;
  bool      _visible;
};


class ComponentEngineExhaustFX : public Component
{
public:
  ComponentEngineExhaustFX(ObjectSpaceship * spaceship, unsigned int engine_id);
  void Tick(double deltatime) override;

private:
  ObjectSpaceship * _spaceship;
  unsigned int      _engine_id;
  double            _current;
};

#endif
