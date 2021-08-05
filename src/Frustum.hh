#ifndef FRUSTUM_HH_
#define FRUSTUM_HH_
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
#include <array>

class Camera;
class Plane;

class Frustum
{
public:
  enum class Result
    {
      INSIDE,
      INTERSECT,
      OUTSIDE
    };
  

  Frustum();

  void UpdateFrustum(const Camera & camera);
  void UpdateView(const Camera & camera);
  Result IsSphereInside(const glm::vec3 & position, double radius) const;
  
private:
  std::array<Plane *, 6> _planes;
  double _ratio;
  double _angle;
  double _tang; // tan(_angle)
  double _near;
  double _far;
  double _near_width, _near_height;
  double _far_width,  _far_height;
  double _sphere_factor_x, _sphere_factor_y;
  glm::vec3 _camera_position;
  glm::vec3 _camera_x_axis;
  glm::vec3 _camera_y_axis;
  glm::vec3 _camera_z_axis;
};

#endif
