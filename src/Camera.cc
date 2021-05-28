#include "Camera.hh"
#include "SubsystemSettings.hh"
#include <iostream>


Camera::Camera()
  : _up(0, 0, 1),
    _position(0, -20, 14.5),
    _target_position(0, 0, 0),
    _view_projection(1)
{
  SetFOV(60.0);
  UpdateViewProjection();
}


Camera::~Camera()
{
}


void Camera::SetFOV(double fov)
{
  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");
  _fov = fov;
  _projection = glm::perspective(glm::radians(_fov), width / height, 0.001, 2000.0);
}


void Camera::SetUp(const glm::vec3 & up)
{
  _up = up;
}


void Camera::SetPosition(const glm::vec3 & position)
{
  _position = position;
}


void Camera::SetTargetPosition(const glm::vec3 & position)
{
  _target_position = position;
}


const glm::mat4 & Camera::GetViewProjection() const
{
  return _view_projection;
}


void Camera::UpdateViewProjection()
{
  _view = glm::lookAt(_position, _target_position, _up);
  _view_projection = _projection * _view;
  std::cout << "camera: " << _position << " -> " << _target_position << std::endl;
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
  UpdateViewProjection();
}


void Camera::MoveRight(double amount)
{
  glm::vec3 d;

  GetDirection(d);
  auto right = glm::normalize(glm::cross(_up, d));
  _position += right * static_cast<float>(amount);
  //_target_position += right * static_cast<float>(amount);
  UpdateViewProjection();
}


void Camera::MoveUp(double amount)
{
  _position += _up * static_cast<float>(amount);
  //_target_position += _up * static_cast<float>(amount);
  UpdateViewProjection();
}


void Camera::GetDirection(glm::vec3 & direction) const
{
  direction = glm::normalize(_target_position - _position);
}

