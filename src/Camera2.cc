/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Camera2.hh"
#include <iostream>

Camera2::Camera2()
  : Camera()
{
  _distance = 3.0;
  _angle = 0.0;
  _tilt = 0.0;

  UpdateViewProjection();
}

void Camera2::MoveForward(double amount)
{
  _distance -= amount;
  UpdateViewProjection();
}


void Camera2::MoveRight(double amount)
{
  _angle += amount;
  std::cout << "angle:" << _angle << std::endl;
  UpdateViewProjection();
}

void Camera2::MoveUp(double amount)
{
  _tilt += amount;
  UpdateViewProjection();
}

void Camera2::UpdateViewProjection()
{
  SetPosition(glm::vec3(0, -_distance, 0)); //_tilt));
  SetTargetPosition(glm::vec3(0, 0, _tilt));

  glm::vec3 up(0, 0, 1);
  up = glm::normalize(glm::rotateY(up, static_cast<float>(glm::radians(_angle))));
  SetUp(up);
  
  Camera::UpdateViewProjection();
}

