#include "Camera.hh"

Camera::Camera()
  : _position(0, 0, 0),
    _target_position(0, 1, 0),
    _view_projection(1)
{
  this->_projection = glm::perspective(60.0, 1024.0 / 768.0, 0.01, 1000.0);
}


void Camera::SetPosition(const glm::vec3 & position)
{
  this->_position = position;
  this->UpdateViewProjection();
}


void Camera::SetTargetPosition(const glm::vec3 & position)
{
  this->_target_position = position;
  this->UpdateViewProjection();
}


const glm::mat4 & Camera::GetViewProjection() const
{
  return this->_view_projection;
}


void Camera::UpdateViewProjection()
{
  this->_view = glm::lookAt(this->_position, this->_target_position, glm::vec3(0, 0, 1));
  this->_view_projection = this->_projection * this->_view;
}

const glm::mat4 & Camera::GetView() const
{
  return this->_view;
}


const glm::mat4 & Camera::GetProjection() const
{
  return this->_projection;
}

