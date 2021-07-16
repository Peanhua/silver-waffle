#ifndef OBJECT_PLANET_ATMOSPHERE_HH_
#define OBJECT_PLANET_ATMOSPHERE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Object.hh"


class ObjectPlanetAtmosphere : public Object
{
public:
  ObjectPlanetAtmosphere(Scene * scene, const glm::vec2 & size, const glm::vec3 & bottom_color, const glm::vec3 & top_color);
};

#endif
