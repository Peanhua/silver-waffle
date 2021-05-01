#ifndef CAMERA2_HH_
#define CAMERA2_HH_

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
