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
    _position(0, -1, 0),
    _target_position(0, 0, 0),
    _view_projection(1),
    _projection_dirty(true),
    _view_dirty(true)
{
  Update();
}


Camera::~Camera()
{
}


void Camera::SetFOV(double fov)
{
  _fov = fov;
  _projection_dirty = true;
}


void Camera::SetClippingPlanes(double near, double far)
{
  _clippingplane_near = near;
  _clippingplane_far  = far;
  _projection_dirty = true;
}


void Camera::SetUp(const glm::vec3 & up)
{
  _up = up;
  _view_dirty = true;
}


void Camera::SetPosition(const glm::vec3 & position)
{
  _position = position;
  _view_dirty = true;
}


void Camera::SetTargetPosition(const glm::vec3 & position)
{
  _target_position = position;
  _view_dirty = true;
}


const glm::mat4 & Camera::GetViewProjection() const
{
  assert(!_projection_dirty);
  assert(!_view_dirty);
  return _view_projection;
}


void Camera::Update()
{
  if(_projection_dirty)
    {
      const double width  = Settings->GetInt("screen_width");
      const double height = Settings->GetInt("screen_height");
      _projection = glm::perspective(glm::radians(_fov), width / height, _clippingplane_near, _clippingplane_far);
    }

  if(_view_dirty)
    _view = glm::lookAt(_position, _target_position, _up);

  if(_projection_dirty || _view_dirty)
    _view_projection = _projection * _view;

  if(_projection_dirty)
    _frustum.UpdateFrustum(*this);
  if(_projection_dirty || _view_dirty)
    _frustum.UpdateView(*this);

  _projection_dirty = false;
  _view_dirty       = false;
}


const glm::mat4 & Camera::GetView() const
{
  assert(!_view_dirty);
  return _view;
}


const glm::mat4 & Camera::GetProjection() const
{
  assert(!_projection_dirty);
  return _projection;
}


void Camera::MoveForward(double amount)
{
  glm::vec3 d;

  GetDirection(d);
  _position += d * static_cast<float>(amount);
  //_target_position += d * static_cast<float>(amount);
  _view_dirty = true;
}


void Camera::MoveRight(double amount)
{
  glm::vec3 d;

  GetDirection(d);
  auto right = glm::normalize(glm::cross(_up, d));
  _position += right * static_cast<float>(amount);
  //_target_position += right * static_cast<float>(amount);
  _view_dirty = true;
}


void Camera::MoveUp(double amount)
{
  _position += _up * static_cast<float>(amount);
  //_target_position += _up * static_cast<float>(amount);
  _view_dirty = true;
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

  auto r = _frustum.IsSphereInside(position, bounding_sphere_radius);
  auto rv = r != Frustum::Result::OUTSIDE;

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

