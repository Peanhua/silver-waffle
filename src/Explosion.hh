#ifndef EXPLOSION_HH_
#define EXPLOSION_HH_
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
#include <random>

class Camera;
class Mesh;


class Explosion
{
public:
  Explosion(std::minstd_rand & random);

  void Activate(const glm::vec3 & position, const glm::vec3 & velocity);
  void Tick(double deltatime);
  void Draw(const Camera & camera) const;
  bool IsAlive() const;
  void Translate(const glm::vec3 & translation);

private:
  int       _fragment_count;
  Mesh *    _mesh;
  double    _time;
  glm::vec3 _position;
  glm::vec3 _velocity;
  glm::mat4 _model;
};

#endif
