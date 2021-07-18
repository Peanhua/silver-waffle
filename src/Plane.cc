/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Plane.hh"


Plane::Plane(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
{
  Set(v0, v1, v2);
}


void Plane::Set(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
{
  auto a = v0 - v1;
  auto b = v2 - v1;
  Set(glm::normalize(glm::cross(b, a)), v1);
}


void Plane::Set(glm::vec3 normal, glm::vec3 point)
{
  _normal = normal;
  _point = point;
  _distance = -(glm::dot(_normal, _point));
}


void Plane::SetCoefficients(float a, float b, float c, float d)
{
  _normal = glm::vec3(a, b, c);
  auto len = glm::length(_normal);
  _normal /= len;
  _distance = d / len;
}


float Plane::GetDistanceToPosition(const glm::vec3 & position) const
{
  return glm::dot(_normal, position) + _distance;
}
