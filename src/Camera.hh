#ifndef CAMERA_HH_
#define CAMERA_HH_
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


class Camera
{
public:
  Camera();
  virtual ~Camera();

  const glm::vec3 & GetPosition() const;
  void              SetPosition(const glm::vec3 & position);
  const glm::vec3 & GetTargetPosition() const;
  void              SetTargetPosition(const glm::vec3 & position);
  double            GetFOV() const;
  void              SetFOV(double fov);
  void              GetClippingPlanes(double & near, double & far) const;
  void              SetClippingPlanes(double near, double far);
  const glm::vec3 & GetUp() const;
  void              SetUp(const glm::vec3 & up);
  
  virtual void      MoveForward(double amount);
  virtual void      MoveRight(double amount);
  virtual void      MoveUp(double amount);

  const glm::mat4 & GetViewProjection() const;
  const glm::mat4 & GetView() const;
  const glm::mat4 & GetProjection() const;
  void              GetDirection(glm::vec3 & direction) const;

  bool              IsInView(const glm::vec3 & position, float bounding_sphere_radius) const;
  void              SetFrustumCulling(bool enabled);

  void              Update();
  
  static struct Stats
  {
    Stats()
      : elapsed_frames(0),
        in_view(0),
        out_view(0)
    {
    }

    uint64_t elapsed_frames;
    uint64_t in_view;
    uint64_t out_view;
  } stats;
  

private:
  Frustum   _frustum;
  bool      _frustum_culling;
  double    _fov;
  double    _clippingplane_near;
  double    _clippingplane_far;
  glm::vec3 _up;
  glm::vec3 _position;
  glm::vec3 _target_position;
  glm::mat4 _view_projection;
  glm::mat4 _projection;
  bool      _projection_dirty;
  glm::mat4 _view;
  bool      _view_dirty;
};

#endif
