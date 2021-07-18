/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Frustum.hh"
#include "Camera.hh"
#include "Plane.hh"
#include "SubsystemSettings.hh"


Frustum::Frustum()
{
  for(unsigned int i = 0; i < _planes.size(); i++)
    _planes[i] = nullptr;
}


void Frustum::UpdateFrustum(const Camera & camera)
{
  const double width  = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");
  _ratio = width / height;
  _angle = glm::radians(camera.GetFOV()) * 0.5;
  camera.GetClippingPlanes(_near, _far);

  _tang = std::tan(_angle);
  _sphere_factor_y = 1.0 / std::cos(_angle); // tang * sin(angle) + cos(angle);

  auto anglex = std::atan(_tang * _ratio);
  _sphere_factor_x = 1.0 / std::cos(anglex); // tang * ratio * sin(anglex) + cos(anglex);

  _near_height = _near * _tang;
  _near_width = _near_height * _ratio; 

  _far_height = _far * _tang;
  _far_width = _far_height * _ratio;
}


void Frustum::UpdateView(const Camera & camera)
{
  _camera_position = camera.GetPosition();
  // compute the Z axis of camera
  // this axis points in the opposite direction from
  // the looking direction
  _camera_z_axis = glm::normalize(camera.GetPosition() - camera.GetTargetPosition());

  // X axis of camera with given "up" vector and Z axis
  _camera_x_axis = glm::normalize(glm::cross(camera.GetUp(), _camera_z_axis));

  // the real "up" vector is the cross product of Z and X
  _camera_y_axis = glm::cross(_camera_z_axis, _camera_x_axis);

  // compute the centers of the near and far planes
  camera.GetClippingPlanes(_near, _far);
  auto nc = camera.GetPosition() - _camera_z_axis * static_cast<float>(_near);
  auto fc = camera.GetPosition() - _camera_z_axis * static_cast<float>(_far);

  auto nw = static_cast<float>(_near_width);
  auto nh = static_cast<float>(_near_height);
  // compute the 4 corners of the frustum on the near plane
  auto ntl = nc + _camera_y_axis * nh - _camera_x_axis * nw;
  auto ntr = nc + _camera_y_axis * nh + _camera_x_axis * nw;
  auto nbl = nc - _camera_y_axis * nh - _camera_x_axis * nw;
  auto nbr = nc - _camera_y_axis * nh + _camera_x_axis * nw;

  auto fw = static_cast<float>(_far_width);
  auto fh = static_cast<float>(_far_height);
  auto ftl = fc + _camera_y_axis * fh - _camera_x_axis * fw;
  auto fbr = fc - _camera_y_axis * fh + _camera_x_axis * fw;
  auto ftr = fc + _camera_y_axis * fh + _camera_x_axis * fw;
  auto fbl = fc - _camera_y_axis * fh - _camera_x_axis * fw;
  
  for(auto plane : _planes)
    delete plane;
  // compute the six planes
  // counter clockwise order
  _planes[0] = new Plane(ntr, ntl, ftl); // top
  _planes[1] = new Plane(nbl, nbr, fbr); // bottom
  _planes[2] = new Plane(ntl, nbl, fbl); // left
  _planes[3] = new Plane(nbr, ntr, fbr); // right
  _planes[4] = new Plane(ntl, ntr, nbr); // near
  _planes[5] = new Plane(ftr, ftl, fbl); // far
}


Frustum::Result Frustum::IsSphereInside(const glm::vec3 & position, double radius) const
{
  auto v = position - _camera_position;
  
  double az = glm::dot(v, -_camera_z_axis);
  if(az > _far + radius || az < _near - radius)
    return Result::OUTSIDE;
  
  double ax = glm::dot(v, _camera_x_axis);
  auto zz1 = az * _tang * _ratio;
  auto d1 = _sphere_factor_x * radius;
  if(ax > zz1 + d1 || ax < -zz1 - d1)
    return Result::OUTSIDE;

  double ay = glm::dot(v, _camera_y_axis);
  auto zz2 = az * _tang;
  auto d2 = _sphere_factor_y * radius;
  if(ay > zz2 + d2 || ay < -zz2 - d2)
    return Result::OUTSIDE;

  
  if(az > _far - radius || az < _near + radius)
    return Result::INTERSECT;
  if(ay > zz2 - d2 || ay < -zz2 + d2)
    return Result::INTERSECT;
  if(ax > zz1 - d1 || ax < -zz1 + d1)
    return Result::INTERSECT;

  
  return Result::INSIDE;
}

