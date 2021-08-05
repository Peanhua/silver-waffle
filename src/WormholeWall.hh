#ifndef WORMHOLE_WALL_HH_
#define WORMHOLE_WALL_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"

class Mesh;


class WormholeWall
{
public:
  WormholeWall(double length, double height);

  void Tick(double deltatime);
  void Draw(const glm::mat4 & model, const glm::mat4 & mvp) const;
  
private:
  Mesh * _mesh;
};

#endif
