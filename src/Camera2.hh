#ifndef CAMERA2_HH_
#define CAMERA2_HH_
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

class Camera2 : public Camera
{
public:
  Camera2();
  
  void MoveForward(double amount) override;
  void MoveRight(double amount) override;
  void MoveUp(double amount) override;

  void UpdateViewProjection() override;
  
private:
  double _distance;
  double _angle;
  double _tilt;
};

#endif
