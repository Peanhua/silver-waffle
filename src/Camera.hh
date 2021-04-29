#ifndef CAMERA_HH_
#define CAMERA_HH_

#include "glm.hh"

class Camera
{
public:
  Camera();
  
  void              SetPosition(const glm::vec3 & position);
  void              SetTargetPosition(const glm::vec3 & position);

  const glm::mat4 & GetViewProjection() const;
  const glm::mat4 & GetView() const;
  const glm::mat4 & GetProjection() const;
  
private:
  glm::vec3 _position;
  glm::vec3 _target_position;
  glm::mat4 _view_projection;
  glm::mat4 _projection;
  glm::mat4 _view;

  void UpdateViewProjection();
};

#endif
