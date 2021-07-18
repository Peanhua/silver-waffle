/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Camera.hh"
#include "SubsystemSettings.hh"
#include <iostream>


Camera::Stats Camera::stats;


Camera::Camera()
  : _frustum_culling(true),
    _fov(60.0),
    _clippingplane_near(0.001),
    _clippingplane_far(2000.0),
    _up(0, 0, 1),
    _position(0, -20, 14.5),
    _target_position(0, 0, 0),
    _view_projection(1)
{
  UpdateView();
  UpdateProjection();
}


Camera::~Camera()
{
}


void Camera::SetFOV(double fov)
{
  _fov = fov;
  UpdateProjection();
}


void Camera::SetClippingPlanes(double near, double far)
{
  _clippingplane_near = near;
  _clippingplane_far  = far;
  UpdateProjection();
}


void Camera::SetUp(const glm::vec3 & up)
{
  _up = up;
  UpdateView();
}


void Camera::SetPosition(const glm::vec3 & position)
{
  _position = position;
  UpdateView();
}


void Camera::SetTargetPosition(const glm::vec3 & position)
{
  _target_position = position;
  UpdateView();
}


const glm::mat4 & Camera::GetViewProjection() const
{
  return _view_projection;
}


void Camera::UpdateProjection()
{
  const double width  = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");
  _projection = glm::perspective(glm::radians(_fov), width / height, _clippingplane_near, _clippingplane_far);
  _view_projection = _projection * _view;
  _frustum.UpdateFrustum(*this);
}


void Camera::UpdateView()
{
  _view = glm::lookAt(_position, _target_position, _up);
  _view_projection = _projection * _view;
  _frustum.UpdateView(*this);
}

const glm::mat4 & Camera::GetView() const
{
  return _view;
}


const glm::mat4 & Camera::GetProjection() const
{
  return _projection;
}


void Camera::MoveForward(double amount)
{
  glm::vec3 d;

  GetDirection(d);
  _position += d * static_cast<float>(amount);
  //_target_position += d * static_cast<float>(amount);
  UpdateView();
}


void Camera::MoveRight(double amount)
{
  glm::vec3 d;

  GetDirection(d);
  auto right = glm::normalize(glm::cross(_up, d));
  _position += right * static_cast<float>(amount);
  //_target_position += right * static_cast<float>(amount);
  UpdateView();
}


void Camera::MoveUp(double amount)
{
  _position += _up * static_cast<float>(amount);
  //_target_position += _up * static_cast<float>(amount);
  UpdateView();
}


void Camera::GetDirection(glm::vec3 & direction) const
{
  direction = glm::normalize(_target_position - _position);
}


const glm::vec3 & Camera::GetPosition() const
{
  return _position;
}


const glm::vec3 & Camera::GetTargetPosition() const
{
  return _target_position;
}


bool Camera::IsInView(const glm::vec3 & position, float bounding_sphere_radius) const
{
  if(!_frustum_culling)
    return true;
  
  auto rv = _frustum.IsSphereInside(position, bounding_sphere_radius) != Frustum::Result::OUTSIDE;

  if(rv)
    Camera::stats.in_view++;
  else
    Camera::stats.out_view++;
  
  return rv;
}


const glm::vec3 & Camera::GetUp() const
{
  return _up;
}


double Camera::GetFOV() const
{
  return _fov;
}


void Camera::GetClippingPlanes(double & near, double & far) const
{
  near = _clippingplane_near;
  far  = _clippingplane_far;
}


void Camera::SetFrustumCulling(bool enabled)
{
  _frustum_culling = enabled;
}

