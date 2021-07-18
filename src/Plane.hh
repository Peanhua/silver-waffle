#ifndef PLANE_HH_
#define PLANE_HH_
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


class Plane
{
public:
  Plane(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2);

  void Set(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2);
  void Set(glm::vec3 normal, glm::vec3 point);
  void SetCoefficients(float a, float b, float c, float d);

  float GetDistanceToPosition(const glm::vec3 & position) const;

private:
  glm::vec3 _normal;
  glm::vec3 _point;
  float     _distance;
};

#endif
